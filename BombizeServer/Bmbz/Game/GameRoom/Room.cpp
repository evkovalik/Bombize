#include <iostream>
#include "RoomGroup.hpp"
#include "Server/UserConnection.hpp"
#include "Game/GameObjects/Map.hpp"
#include "Game/GameObjects/Avatar.hpp"
#include "Game/Structs/GroupToGame.hpp"
#include "Generators/Random.hpp"
#include "Resources/ResourceLocator.hpp"

namespace Bmbz
{
namespace Game
{
    Room::Room(RoomGroup& group, uint16_t numberRoom)
    : _group(group)
    , _number(numberRoom)
    , _timerUpdateLoop(group.Strand())
    , _gameContext(group.Strand())
    {}

    Room::~Room()
    {
        _timerUpdateLoop.cancel();
    }

    void Room::Run(
        GroupToGame&& gamers,
        std::function<void(int)> callbackFinish)
    {
        _callbackFinishGame = callbackFinish;
        
        auto mapId = Generator::Random::RangeInt(
            0, static_cast<int>(ResourceLocator::GetMaps().Count() - 1)
        );
        auto mapInfo = ResourceLocator::GetMaps().GetMapInfo(mapId);
        NetworkData::RoomData roomData{mapInfo};
        std::function<void(int)> func;

        func = [&](int number)
        {
            if(gamers.Count() > 0)
            {
                auto userConnection = gamers.ExtractNextGamer();
                auto accessKey = _genAccessKeys.GetValueUInt32();
                _participants.emplace_back(
                    ParticipantNode
                    {
                        .accessKey = accessKey,
                        .item = Participant{number, userConnection, _group.UdpMaster()},
                        .winningPlace = 0
                    }
                );
                roomData.AddPlayerData(number, userConnection->GetUserInfo());
                func(++number);

                --number;
                roomData.playerSettings = NetworkData::PlayerSettings{accessKey, _number, static_cast<uint8_t>(number)};
                userConnection->InformAboutStartGame(
                    roomData,
                    boost::asio::bind_executor(_group.Strand(),
                        [this](std::shared_ptr<UserConnection> user){ OnClosingRoom(user); }
                    )
                );
            }
        };

        if(gamers.Count() > 0)
        {
            func(0);
            CreateWorld(std::move(mapInfo));
            Start();
        }
        else _callbackFinishGame(_number);
    }

    void Room::CreateWorld(Resource::MapInfo&& mapInfo)
    {
        auto map = _gameContext.Objects().CreateObject<Map>();
        map->Init(std::move(mapInfo));

        auto& spawnPos = map->GetSpawnPositions();
        int spawnPosIndex = 0;
        for(auto& node : _participants)
        {
            auto avatar = _gameContext.Objects().CreateObject<Avatar>();
            avatar->SetStartPosition(
                NetType::Vector2<float>{spawnPos[spawnPosIndex].x, spawnPos[spawnPosIndex].y},
                map
            );
            node.item.SetAvatar(avatar, [this](Participant& p){ EventDeathOfAvata(p); });
            spawnPosIndex++;
        }
    }

    void Room::OnClosingRoom(std::shared_ptr<UserConnection> user)
    {
        for(auto it = _participants.begin(); it != _participants.end(); ++it)
        {
            if(it->item.GetUserConnection() == user)
            {
                ReleaseParticipant(*it);
                _participants.erase(it);
                break;
            }
        }
    }

    void Room::ReleaseParticipant(ParticipantNode& node)
    {
        if(node.item.HasAvatar())
        {
            node.item.KillAvatar();
            if(node.winningPlace == 0) node.winningPlace = _nextWinningPlace--;
        }
        auto connection = node.item.GetUserConnection();
        connection->InformAboutFinishGame(
            _rewardCalculator.GetReward(connection->GetUserInfo(), node.winningPlace)
        );
    }

    void Room::EventDeathOfAvata(Participant& participant)
    {
        for(auto& node : _participants)
            if(&(node.item) == &participant)
            {
                node.winningPlace = _nextWinningPlace--;
                break;
            }
    }

    void Room::Start()
    {
        _gameStartTime = std::chrono::steady_clock::now();

        _nextWinningPlace = static_cast<int>(_participants.size());
        _rewardCalculator.Reserve(_participants.size());
        for(auto node : _participants)
            _rewardCalculator.AddToCalculations(node.item.GetUserConnection()->GetUserInfo());

        _timerUpdateLoop.expires_after(START_DELAY_TIME);
        _timerUpdateLoop.async_wait(
            [this](boost::system::error_code ec)
            {
                if (!ec)
                {
                    _gameState = GameState::Running;
                    Update();
                }
                else ErrorFinishGame();
            }
        );
    }

    void Room::Update()
    {
        _timerUpdateLoop.expires_after(LOOP_TIME);    // 60FPS ~ 16ms
        _timerUpdateLoop.async_wait(
            [this](boost::system::error_code ec)
            {
                if (!ec)
                {
                    GameLoopIteration();
                    if(_nextWinningPlace > 1)
                    {
                        Update();
                    }
                    else
                    {
                        FinishGame();
                    }
                }
            }
        );
    }

    void Room::GameLoopIteration()
    {
        _frameStartTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(_frameEndTime - _frameStartTime).count();

        _gameContext.Objects().UpdateAll(deltaTime);
        _gameContext.Collisions().Calculate();
        SendGameActions();

        _frameEndTime = std::chrono::steady_clock::now();
    }

    void Room::SendGameActions()
    {
        for(auto it = _participants.begin(); it != _participants.end(); )
        {
            if(it->item.IsConnectionUDP())
            {
                it->item.SendOldActions();
                ++it;
            }
            else
            {
                ReleaseParticipant(*it);
                it = _participants.erase(it);
            }
        }
        if(_gameContext.Writer().BufferSize() > 0)
        {
            for(auto& node : _participants)
                node.item.SendActions(
                    _gameContext.Writer().GetBuffer(), _gameContext.Writer().ConfirmationFlag()
                );
            _gameContext.Writer().ClearBuffer();
        }
    }

    void Room::FinishGame()
    {
        for(auto& node : _participants)
        {
            if(node.winningPlace == 0)
            {
                if(node.item.HasAvatar()) node.item.KillAvatar();
                node.winningPlace = _nextWinningPlace--;
            }
            auto user = node.item.GetUserConnection();
            user->InformAboutFinishGame(
                _rewardCalculator.GetReward(user->GetUserInfo(), node.winningPlace)
            );
        }
        _participants.clear();
		_callbackFinishGame(_number);
    }

    void Room::ErrorFinishGame()
    {
        for(auto& node : _participants)
        {
            if(node.item.HasAvatar()) node.item.KillAvatar();

            node.winningPlace = 0;
            auto user = node.item.GetUserConnection();
            user->InformAboutFinishGame(
                _rewardCalculator.GetReward(user->GetUserInfo(), node.winningPlace)
            );
        }
        _participants.clear();
		_callbackFinishGame(_number);
    }

    void Room::ProcessingInputData(
        boost::beast::flat_buffer& inBuffer,
        const boost::asio::ip::udp::endpoint& remoteEndpoint)
    {
        uint32_t key;
        Utils::ReadFromBuffer(inBuffer, key);
        for(auto& node : _participants)
        {
            if(node.accessKey == key)
            {
                node.item.ProcessInputMessage(inBuffer, remoteEndpoint);
            }
        }
    }

} // namespase Game
}