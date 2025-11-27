#include "TcpListener.hpp"

namespace Bmbz
{
namespace Network
{
    TcpListener::TcpListener(
        boost::asio::io_context& ioc,
        boost::asio::ip::tcp::endpoint endpoint)
    : _ioc(ioc)
    , _acceptor(ioc)
    {
        boost::beast::error_code ec;

        _acceptor.open(endpoint.protocol(), ec);
        if (ec)
        {
            return;
        }

        _acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
        if (ec)
        {
            return;
        }

        _acceptor.bind(endpoint, ec);
        if (ec)
        {
            return;
        }

        _acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
        if (ec)
        {
            return;
        }
    }

    void TcpListener::Run()
    {
        DoAccept();
    }

    void TcpListener::DoAccept()
    {
        _acceptor.async_accept(
            boost::asio::make_strand(_ioc),
            boost::beast::bind_front_handler(
                &TcpListener::HandleAccept,
                shared_from_this())
        );
    }

    void TcpListener::HandleAccept(
        const boost::beast::error_code& ec,
        boost::asio::ip::tcp::socket socket)
    {
        if (ec)
        {

        }
        else if(_otherConnectionHandler)
        {
            _otherConnectionHandler(std::move(socket));
        }
        DoAccept();
    }

    // void TcpListener::OnAccept(boost::asio::ip::tcp::socket&& socket)
    // {
    //     //std::make_shared<WebSocketConnection>(std::move(socket))->Run();
    // }
}
}