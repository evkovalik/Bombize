#include "UserConnection.hpp"

namespace Bmbz
{
    UserConnection::UserConnection(
        boost::asio::ip::tcp::socket&& socket,
        ServerServices& services
    )
    : WebSocketConnection(std::move(socket))
    , _services(services)
    {}

    void UserConnection::OnDisconnect()
    {
        if(_mode == UserMode::WaitingGame)
        {
            bool success = _services.GetMatchOrganizer().CancelRegistrationForGame(SelfSharedPtr());
            if(success && _userInfo) _services.GetGameDB().AsyncUpdateUserData(_userInfo);
        }
        else if(_mode == UserMode::Lobby && _userInfo)
        {
            _services.GetGameDB().AsyncUpdateUserData(_userInfo);
        }
        // else inaction. waiting kick from room

        _mode = UserMode::Exit;
        std::cout << "UserConnection::OnDisconnect" << std::endl;
    }

    // <========== public ==========> //

    void UserConnection::InformAboutStartGame(
        const Game::NetworkData::RoomData& roomData,
        std::function<void(std::shared_ptr<UserConnection>)> roomCloser)
    {
        auto handler =
        [self = SelfSharedPtr(), roomData, roomCloser = std::move(roomCloser)]()
        {
            if(self->_mode == UserMode::Exit) return; // waiting kick from room

            self->_gameRoomCloser = roomCloser;
            auto outBuffer = self->_bufferPool.GetAndOccupyFreeBuffer();

            Utils::WriteToBuffer(outBuffer->body, SrvMsgType::RunGame);
            Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);
            roomData.Serialize(outBuffer->body);
            self->SendData(outBuffer);
            self->_mode = UserMode::Game;
        };
        boost::asio::post(GetStrand(), std::move(handler));
    }

    void UserConnection::InformAboutFinishGame(Game::NetworkData::RewardGame reward)
    {
        auto handler =
        [self = SelfSharedPtr(), reward = std::move(reward)]()
        {
            auto userInfo = self->GetUserInfo();
            userInfo->experience += reward.experience;
            userInfo->coinsC += reward.coins;
            userInfo->rating += reward.rating;

            if(self->_mode == UserMode::Exit)
            {
                if(self->_userInfo) self->_services.GetGameDB().AsyncUpdateUserData(self->_userInfo);
            }
            else
            {
                self->_gameRoomCloser = nullptr;
                auto outBuffer = self->_bufferPool.GetAndOccupyFreeBuffer();

                Utils::WriteToBuffer(outBuffer->body, SrvMsgType::ResultGame);
                Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);
                reward.Serialize(outBuffer->body);
                self->SendData(outBuffer);
                self->_mode = UserMode::Lobby;
            }
        };
        boost::asio::post(GetStrand(), std::move(handler));
    }

    // <========== end public ==========> //

    void UserConnection::OnMessage(boost::beast::flat_buffer& buffer)
    {
        if (buffer.size() == 0) return;

        CltMsgType msgType;
        Utils::ReadFromBuffer(buffer, msgType);
        std::cout << "CltMsgType: " << (int)msgType << ". Mode: " << (int)_mode << std::endl;

        if(_mode == UserMode::Lobby)
        {
            if(msgType == CltMsgType::Play)
            {
                ToGame();
            }
            else if(msgType == CltMsgType::GetUserInfo)
            {
                SendActualUserInfo();
            }
            else if(msgType == CltMsgType::Buy) // temp
            {
                uint8_t index;
                Shop::CoinType coinType;
                Utils::ReadFromBuffer(buffer, index);
                Utils::ReadFromBuffer(buffer, coinType);

                auto outBuffer = _bufferPool.GetAndOccupyFreeBuffer();
                Utils::WriteToBuffer(outBuffer->body, SrvMsgType::UserInfo);
                if(_services.GetShop().BuyCharacter(_userInfo, index, coinType))
                {
                    Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);
                    _userInfo->Serialize(outBuffer->body);
                }
                else
                    Utils::WriteToBuffer(outBuffer->body, MsgStatus::Error);
                SendData(outBuffer);
            }
            else if(msgType == CltMsgType::SetCharacter) // temp
            {
                uint8_t index;
                Utils::ReadFromBuffer(buffer, index);
                auto outBuffer = _bufferPool.GetAndOccupyFreeBuffer();
                Utils::WriteToBuffer(outBuffer->body, SrvMsgType::UserInfo);
                
                _userInfo->UpdateCharactersList();
                if(0 <= index && index <= static_cast<uint8_t>(_userInfo->characters.size()))
                {
                    Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);
                    _userInfo->activeCharacterIndex = index;
                }
                else
                    Utils::WriteToBuffer(outBuffer->body, MsgStatus::Error);
                
                _userInfo->Serialize(outBuffer->body); // if error: update client state
                SendData(outBuffer);
            }
        }
        else if(_mode == UserMode::Game || _mode == UserMode::WaitingGame)
        {
            if(msgType == CltMsgType::CloseGameRoom)
            {
                LeaveGame();
            }
        }
        else if(_mode == UserMode::Unknown)
        {
            if(msgType == CltMsgType::Login)
                Authorization(buffer);
            else if(msgType == CltMsgType::NewUser)
                CreateNewAccount();
        }
    }

    void UserConnection::Authorization(boost::beast::flat_buffer& buffer)
    {
        std::cout << "Authorization" << std::endl;
        User::LoginData loginData;
        loginData.Deserialize(buffer);

        auto handleUserInfoFromDB = 
        [self = SelfSharedPtr()]
        (std::shared_ptr<User::UserInfo> userInfo)
        {
            auto outBuffer = self->_bufferPool.GetAndOccupyFreeBuffer();
            Utils::WriteToBuffer(outBuffer->body, SrvMsgType::Account);
            if(userInfo)
            {
                self->_userInfo = userInfo;
                Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);

                self->_userInfo->Serialize(outBuffer->body);
                self->_services.GetShop().SerializeItemsForSale(outBuffer->body);
                self->_mode = UserMode::Lobby;
            }
            else
            {
                Utils::WriteToBuffer(outBuffer->body, MsgStatus::Error);
                self->_mode = UserMode::Blocked;
            }
            self->SendData(outBuffer);
        };
        _services.GetGameDB().AsyncGetUserData(
            std::move(loginData),
            boost::asio::bind_executor(GetStrand(), std::move(handleUserInfoFromDB))
        );
    }

    void UserConnection::CreateNewAccount()
    {
        std::cout << "CreateNewAccount" << std::endl;
        auto handleCreateUser = 
        [self = SelfSharedPtr()]
        (User::LoginData loginData, std::shared_ptr<User::UserInfo> userInfo)
        {
            if(self->_mode == UserMode::Exit)
            {
                // remove created user from db
                return;
            }
            
            auto outBuffer = self->_bufferPool.GetAndOccupyFreeBuffer();
            Utils::WriteToBuffer(outBuffer->body, SrvMsgType::NewAccount);
            if(userInfo)
            {
                self->_userInfo = userInfo;
                Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);
                loginData.Serialize(outBuffer->body);

                self->_userInfo->Serialize(outBuffer->body);
                self->_services.GetShop().SerializeItemsForSale(outBuffer->body);
                self->_mode = UserMode::Lobby;
            }
            else
            {
                Utils::WriteToBuffer(outBuffer->body, MsgStatus::Error);
                self->_mode = UserMode::Blocked;
            }
            self->SendData(outBuffer);
        };
        _services.GetGameDB().AsyncCreateUser(
            boost::asio::bind_executor(GetStrand(), std::move(handleCreateUser))
        );
    }

    void UserConnection::ToGame()
    {
        _services.GetMatchOrganizer().RegisterForGame(SelfSharedPtr());
        _mode = UserMode::WaitingGame;
        auto outBuffer = _bufferPool.GetAndOccupyFreeBuffer();
        Utils::WriteToBuffer(outBuffer->body, SrvMsgType::Confirm);
        Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);
        SendData(outBuffer);
    }

    void UserConnection::LeaveGame()
    {
        auto outBuffer = _bufferPool.GetAndOccupyFreeBuffer();
        Utils::WriteToBuffer(outBuffer->body, SrvMsgType::Confirm);

        if(_mode == UserMode::WaitingGame)
        {
            if(_services.GetMatchOrganizer().CancelRegistrationForGame(SelfSharedPtr()))
            {
                Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);
                _mode = UserMode::Lobby;
            }
            else
                Utils::WriteToBuffer(outBuffer->body, MsgStatus::Error);
        }
        else if(_mode == UserMode::Game && _gameRoomCloser)
        {
            _gameRoomCloser(SelfSharedPtr());
            Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);
        }
        else
        {
            Utils::WriteToBuffer(outBuffer->body, MsgStatus::Error);
        }
        SendData(outBuffer);
    }

    void UserConnection::SendActualUserInfo()
    {
        auto outBuffer = _bufferPool.GetAndOccupyFreeBuffer();
        Utils::WriteToBuffer(outBuffer->body, SrvMsgType::UserInfo);
        Utils::WriteToBuffer(outBuffer->body, MsgStatus::Ok);
        _userInfo->Serialize(outBuffer->body);
        SendData(outBuffer);
    }
}