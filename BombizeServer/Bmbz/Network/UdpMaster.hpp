#ifndef BMBZ_UDPMASTER_HPP
#define BMBZ_UDPMASTER_HPP

#include <boost/asio.hpp>
#include <boost/asio/execution/any_executor.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include "Types/SharedBufferWithHeader.hpp"

namespace Bmbz
{
namespace Network
{
	class UdpMaster
	{
	public:
		using OnReceive = std::function<void(
			boost::beast::flat_buffer&,
			const boost::asio::ip::udp::endpoint&
		)>;

		UdpMaster(
			const boost::asio::any_io_executor& executor,
			boost::asio::ip::udp::endpoint endpoint,
			OnReceive callbackOnReceive
		);

		void SendData(
			std::shared_ptr<Type::SharedBufferWithHeader> sendBuffer,
			boost::asio::ip::udp::endpoint endpoint) const;

		const boost::asio::ip::udp::endpoint& Endpoint() const { return _selfEndpoint; }

	private:
		mutable boost::asio::ip::udp::socket _socket;
		boost::asio::ip::udp::endpoint _selfEndpoint;
		boost::asio::ip::udp::endpoint _remoteEndpoint;
		boost::beast::flat_buffer _receiveBuffer;
		OnReceive _callbackOnReceive;

		void Receive();
	};
}
}
#endif