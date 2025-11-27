#include "UdpMaster.hpp"
#include <iostream>

namespace Bmbz
{
namespace Network
{
    UdpMaster::UdpMaster(
        const boost::asio::any_io_executor& executor,
        boost::asio::ip::udp::endpoint endpoint,
        OnReceive callbackOnReceive
    )   
    : _socket(executor, endpoint)
    , _selfEndpoint(endpoint)
    , _receiveBuffer(256)
    , _callbackOnReceive(callbackOnReceive)
    {
        Receive();
    }

    void UdpMaster::SendData(
        std::shared_ptr<Type::SharedBufferWithHeader> sendBuffer,
        boost::asio::ip::udp::endpoint endpoint) const
    {
        _socket.async_send_to(
            sendBuffer->GetSendData(),
            endpoint,
            [sendBuffer, endpoint = endpoint](boost::system::error_code /*ec*/, std::size_t countBytes)
            {
                sendBuffer->Sent(true);
                // std::cout << "UdpMaster::SendTo: " << endpoint.address().to_string() << ":" << endpoint.port() 
                // << ", bytes: " << countBytes << "\n";
            }
        );
    }

    void UdpMaster::Receive()
    {
        try
        {
            _receiveBuffer.consume(_receiveBuffer.size());
            _socket.async_receive_from(
            _receiveBuffer.prepare(64),
            _remoteEndpoint,
            [this] (const boost::system::error_code& error, std::size_t countBytes)
                {
                    if (!error)
                    {
                        _receiveBuffer.commit(countBytes);
                        _callbackOnReceive(_receiveBuffer, _remoteEndpoint);
                        Receive();
                    }
                    else
                    {
                        //std::cerr << " Error [UdpMaster::Receive::Handler]: " << error.what() << std::endl;
                        Receive();
                    }
                }
            );
        }
        catch(const std::exception& e)
        {
            std::cerr << " Error [UdpMaster::Receive]: " << e.what() << '\n';
        }
    }
}
}