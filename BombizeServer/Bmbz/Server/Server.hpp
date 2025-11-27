#ifndef BMBZ_SERVER_HPP
#define BMBZ_SERVER_HPP

#include <memory>
#include "Config/ServerConfig.hpp"
#include "Network/TcpListener.hpp"
#include "ServerServices/ServerServices.hpp"
#include "UserConnection.hpp"

namespace Bmbz
{
    class Server
    {
    public:
        Server(
            boost::asio::io_context& ioc,
            const Config::ServerConfig& config
        );
        void Run();

    private:
        boost::asio::io_context& _ioc;
        const Config::ServerConfig& _config;
        std::shared_ptr<Network::TcpListener> _listener;
        ServerServices _services;
        
        void HandleConnection(boost::asio::ip::tcp::socket&& socket);
    };
}

#endif