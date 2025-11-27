#ifndef BMBZ_WEBSOCKETCONNECTION_HPP
#define BMBZ_WEBSOCKETCONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <deque>
#include <iostream>
#include "Types/SharedBuffer.hpp"

namespace Bmbz
{
namespace Network
{
    class WebSocketConnection : public std::enable_shared_from_this<WebSocketConnection>
    {
    public:
        explicit WebSocketConnection(boost::asio::ip::tcp::socket&& socket);
        virtual ~WebSocketConnection() = default;

        void Run();
        void SendData(std::shared_ptr<Type::SharedBuffer> sendBuffer);
    protected:
        inline boost::asio::any_io_executor GetStrand() noexcept { return _websocket.get_executor(); }
        virtual void OnMessage(boost::beast::flat_buffer& buffer);
        virtual void OnDisconnect();
    private:
        boost::beast::websocket::stream<boost::beast::tcp_stream> _websocket;
        boost::beast::flat_buffer _buffer;

        std::deque<std::shared_ptr<Type::SharedBuffer>> _writeQueue;
        bool _isWriting = false;

        void HandleRun();
        void HandleAccept(boost::beast::error_code ec); // after websocket connection confirmation
        void Receive();
        void HandleReceive(boost::beast::error_code ec, std::size_t countBytes);
        void Send(std::shared_ptr<Type::SharedBuffer> sendBuffer);
    };
}
}
#endif