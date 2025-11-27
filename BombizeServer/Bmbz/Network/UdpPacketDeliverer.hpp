#ifndef BMBZ_UDPPACKETDELIVERER_HPP
#define BMBZ_UDPPACKETDELIVERER_HPP

#include "UdpMaster.hpp"
#include "Types/DynamicBufferPoolUdp.hpp"
#include "Utils/ReadWriteData.hpp"

namespace Bmbz
{
namespace Network
{
    class UdpPacketDeliverer
    {
    public:
        UdpPacketDeliverer(const UdpMaster& udpMaster)
        : _udpMaster(udpMaster)
        {}

        UdpPacketDeliverer(const UdpMaster& udpMaster, const boost::asio::ip::udp::endpoint& endpoint)
        : _udpMaster(udpMaster)
        , _endpoint(endpoint)
        {}

        const boost::asio::ip::udp::endpoint& Endpoint() const { return _endpoint; }
        bool IsConnection() const;

        void UpdateEndpoint(const boost::asio::ip::udp::endpoint& endpoint);
        bool ProcessingInputPacket(boost::beast::flat_buffer& buffer);
        boost::beast::flat_buffer& GetOutBuffer();
        void SetConfirmOfCurrentPackage(bool confirm);
        void SendCurrentData();
        void RepeatSend();

    private:
        const UdpMaster& _udpMaster;
        boost::asio::ip::udp::endpoint _endpoint;
        Type::DynamicBufferPoolUdp _bufferPool{0, 128};
        std::shared_ptr<Type::SharedBufferWithHeader> _currentOutBuffer = nullptr;
        std::vector<std::pair<uint32_t, std::shared_ptr<Type::SharedBufferWithHeader>>> _unconfirmedPackets;
        uint32_t _nextPackageNumber = 1;
        uint32_t _expectedPackageNumber = 1;

        static constexpr std::chrono::milliseconds _disconnectTime{3000};
        std::chrono::time_point<std::chrono::steady_clock> _lastInputPacketTime{};

        void ConfirmPacket(uint32_t number);
        void ResetSelfState();

        struct SendingSettings
        {
            uint32_t outConfirmationNumber;
            bool needConfirmation;

        } _currentSettings;
    };
}
}
#endif