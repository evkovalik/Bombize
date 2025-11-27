#ifndef BMBZ_GAME_ROOMSMANAGER_HPP
#define BMBZ_GAME_ROOMSMANAGER_HPP

#include "RoomGroup.hpp"
#include "Config/UdpConfig.hpp"

namespace Bmbz
{
namespace Game
{
    class RoomsManager
    {
    public:
        RoomsManager(boost::asio::io_context& ioc, Config::UdpConfig config)
        : _ioc(ioc)
        , _config(std::move(config))
        , _nextPortNumber(_config.portStart)
        , _maxPortNumber(_config.portStart + _config.portCount)
        {}

        void CreateAndRunRoom(GroupToGame gamers)
        {
            bool success = false;
            if(_roomsGroups.size() > 0)
            {
                for(auto& group : _roomsGroups)
                {
                    if(group->Size() < _config.maxRoomsPerPort)
                    {
                        group->CreateAndRunRoom(std::move(gamers));
                        success = true;
                        break;
                    }
                    else continue;
                }
            }

            if(!success)
            {
                if(_nextPortNumber < _maxPortNumber)
                {
                    auto& group = _roomsGroups.emplace_back(std::make_unique<RoomGroup>(
                        _ioc, 
                        boost::asio::ip::udp::endpoint(
                            boost::asio::ip::make_address(_config.host), _nextPortNumber++
                        )
                    ));
                    group->CreateAndRunRoom(std::move(gamers));
                }
                else
                {
                    // Error: no free ports...
                }
            }
        }

    private:
        boost::asio::io_context& _ioc;
        Config::UdpConfig _config;
        unsigned _nextPortNumber;
        const unsigned _maxPortNumber;
        std::vector<std::unique_ptr<RoomGroup>> _roomsGroups;
    };
}
}
#endif