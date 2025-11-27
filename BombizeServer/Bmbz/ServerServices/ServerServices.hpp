#ifndef BMBZ_SERVERSERVICES_HPP
#define BMBZ_SERVERSERVICES_HPP

#include "Config/ServerConfig.hpp"
#include "Database/GameDB.hpp"
#include "MatchOrganizer.hpp"
#include "Shop.hpp"

namespace Bmbz
{
    class ServerServices
    {
    public:
        ServerServices() = delete;
        ServerServices(
            boost::asio::io_context& ioc,
            const Config::ServerConfig& config
        )
        : _matchOrganizer(ioc, std::move(config.GetUdpConfig()))
        , _gameDB(std::move(config.GetDBConfig()))
        {}

        GameDB& GetGameDB() noexcept { return _gameDB; }
        MatchOrganizer& GetMatchOrganizer() noexcept { return _matchOrganizer; }
        Shop& GetShop() noexcept { return _shop; }
    
    private:
        MatchOrganizer _matchOrganizer;
        GameDB _gameDB;
        Shop _shop;
    };
}
#endif