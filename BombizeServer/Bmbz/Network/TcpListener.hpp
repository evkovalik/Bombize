#ifndef BMBZ_TCPLISTENER_HPP
#define BMBZ_TCPLISTENER_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace Bmbz
{
namespace Network
{
    class TcpListener : public std::enable_shared_from_this<TcpListener>
    {
    public:
        TcpListener(boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint);
        
        void Run();
        void SetConnectionHandler(std::function<void(boost::asio::ip::tcp::socket&& socket)> handler)
        {
            _otherConnectionHandler = handler;
        }
        void ResetConnectionHandler()
        {
            _otherConnectionHandler = nullptr;
        }

    private:
        boost::asio::io_context& _ioc;
        boost::asio::ip::tcp::acceptor _acceptor;

        std::function<void(boost::asio::ip::tcp::socket&& socket)> _otherConnectionHandler = nullptr;

        void DoAccept();
        void HandleAccept(const boost::beast::error_code& ec, boost::asio::ip::tcp::socket socket);
    };
}
}


#endif