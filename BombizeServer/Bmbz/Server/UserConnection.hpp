#ifndef BMBZ_USERCONNECTION_HPP
#define BMBZ_USERCONNECTION_HPP

#include <functional>
#include "Network/WebSocketConnection.hpp"
#include "ServerServices/ServerServices.hpp"
#include "MsgType.hpp"
#include "User/UserInfo.hpp"
#include "User/LoginData.hpp"
#include "Types/DynamicBufferPool.hpp"
#include "Game/NetworkData/RoomData/RoomData.hpp"
#include "Game/NetworkData/RewardGame.hpp"

namespace Bmbz
{
    class UserConnection final : public Network::WebSocketConnection
    {
    public:
        enum class UserMode
        {
            Unknown,
            Blocked,
            Authorization,
            Lobby,
            WaitingGame,
            Game,
            Exit
        };

        UserConnection(
            boost::asio::ip::tcp::socket&& socket,
            ServerServices& services
        );

        const auto GetUserInfo() const { return _userInfo; }

        void InformAboutStartGame(
            const Game::NetworkData::RoomData& roomData,
            std::function<void(std::shared_ptr<UserConnection>)> roomCloser
        );
        void InformAboutFinishGame(Game::NetworkData::RewardGame reward);

    private:
        ServerServices& _services;
        Type::DynamicBufferPool _bufferPool{3, 0};

        UserMode _mode = UserMode::Unknown;
        std::shared_ptr<User::UserInfo> _userInfo;
        std::function<void(std::shared_ptr<UserConnection>)> _gameRoomCloser = nullptr;

        inline std::shared_ptr<UserConnection> SelfSharedPtr()
        {
            return std::static_pointer_cast<UserConnection>(shared_from_this());
        }
        void OnMessage(boost::beast::flat_buffer& buffer) override;
        void OnDisconnect() override;

        void CreateNewAccount();
        void Authorization(boost::beast::flat_buffer& buffer);
        void ToGame();
        void LeaveGame();
        void SendActualUserInfo();
    };
}
#endif