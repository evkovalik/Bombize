#include "Server.hpp"

using namespace boost::asio::ip;

namespace Bmbz
{
    Server::Server(
        boost::asio::io_context& ioc,
        const Config::ServerConfig& config
    )
    : _ioc(ioc)
    , _services(ioc, config)
    , _config(config)
    {}

    void Server::Run()
    {
        auto tcpConfig = _config.GetTcpConfig();
        _listener = std::make_shared<Network::TcpListener>(
            _ioc, ::tcp::endpoint{::make_address(tcpConfig.host), tcpConfig.port}
        );
        _listener->SetConnectionHandler(
            [this](boost::asio::ip::tcp::socket&& socket){ HandleConnection(std::move(socket)); }
        );
        _listener->Run();
    }

    void Server::HandleConnection(boost::asio::ip::tcp::socket&& socket)
    {
        std::make_shared<UserConnection>(std::move(socket), _services)->Run();
    }
}