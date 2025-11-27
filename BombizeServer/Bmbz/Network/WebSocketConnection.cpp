#include "WebSocketConnection.hpp"

namespace Bmbz
{
namespace Network
{
    WebSocketConnection::WebSocketConnection(boost::asio::ip::tcp::socket&& socket)
    : _websocket(std::move(socket))
    {}

    void WebSocketConnection::Run()
    {
        boost::asio::dispatch(_websocket.get_executor(),
            boost::beast::bind_front_handler(
                &WebSocketConnection::HandleRun,
                shared_from_this())
        );
    }

    void WebSocketConnection::HandleRun()
    {
        /* Settings websocket connection */
        _websocket.set_option(boost::beast::websocket::stream_base::timeout::suggested(
            boost::beast::role_type::server));

        _websocket.set_option(boost::beast::websocket::stream_base::decorator(
            [](boost::beast::websocket::response_type& res)
            {
                res.set(boost::beast::http::field::server,
                    std::string(BOOST_BEAST_VERSION_STRING) + " server");
            })
        );
        _websocket.binary(true);
        _websocket.async_accept(boost::beast::bind_front_handler(
            &WebSocketConnection::HandleAccept,
            shared_from_this())
        );
    }

    void WebSocketConnection::HandleAccept(boost::beast::error_code ec)
    {
        if(ec)
        {
            return;
        }
        Receive();
    }

    void WebSocketConnection::Receive()
    {
        if(_buffer.size() != 0) _buffer.consume(_buffer.size());
        _websocket.async_read(
            _buffer,
            boost::beast::bind_front_handler(
                &WebSocketConnection::HandleReceive,
                shared_from_this())
        );
    }

    void WebSocketConnection::HandleReceive(boost::beast::error_code ec, std::size_t countBytes)
    {
        boost::ignore_unused(countBytes);
        if(ec) 
        {
            OnDisconnect();
            return;
        }
        OnMessage(_buffer);
        _buffer.clear();
        Receive();
    }

    void WebSocketConnection::OnMessage(boost::beast::flat_buffer& buffer)
    {
        std::cout << "WebSocketConnection::OnMessage" << std::endl;
    }

    void WebSocketConnection::SendData(std::shared_ptr<Type::SharedBuffer> sendBuffer)
    {
        boost::asio::dispatch(_websocket.get_executor(),
            [self = shared_from_this(), sendBuffer]()
            {
                if(self->_isWriting) self->_writeQueue.push_back(sendBuffer);
                else self->Send(sendBuffer);
            }
        );
    }

    void WebSocketConnection::Send(std::shared_ptr<Type::SharedBuffer> sendBuffer)
    {
        _isWriting = true;
        _websocket.async_write(sendBuffer->body.data(),
            [self = shared_from_this(), sendBuffer](boost::beast::error_code ec, std::size_t countBytes)
            {
                sendBuffer->Sent(true);
                self->_isWriting = false;
                if(!self->_writeQueue.empty())
                {
                    auto nextBuffer = self->_writeQueue.front();
                    self->_writeQueue.pop_front();
                    self->Send(nextBuffer);
                }
            }
        );
    }

    void WebSocketConnection::OnDisconnect()
    {
        std::cout << "WebSocketConnection::OnDisconnect" << std::endl;
    }
}
}