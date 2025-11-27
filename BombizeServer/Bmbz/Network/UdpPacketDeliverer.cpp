#include "UdpPacketDeliverer.hpp"

namespace Bmbz
{
namespace Network
{
    void UdpPacketDeliverer::UpdateEndpoint(const boost::asio::ip::udp::endpoint& endpoint)
    {
        if(_endpoint != endpoint) _endpoint = endpoint;
    }

    bool UdpPacketDeliverer::IsConnection() const
    {
        return std::chrono::steady_clock::now() - _lastInputPacketTime <= _disconnectTime;
    }

    bool UdpPacketDeliverer::ProcessingInputPacket(boost::beast::flat_buffer& buffer)
    {
        _lastInputPacketTime = std::chrono::steady_clock::now();
        uint32_t inNumber;
        uint8_t hasConfirmation;
        uint32_t confirmationNumber{0};
        Utils::ReadFromBuffer(buffer, inNumber);
        Utils::ReadFromBuffer(buffer, hasConfirmation);
        
        if(hasConfirmation)
        {
            Utils::ReadFromBuffer(buffer, confirmationNumber);
            ConfirmPacket(confirmationNumber);
        }

        // std::cout << "Input UdpPacke #" << inNumber  << " | " << (int)hasConfirmation << " | " << confirmationNumber
        // << " --- " << _expectedPackageNumber << "\n";

        if(inNumber == 0)
        {
            return buffer.size() > 0;
        }
        else if(inNumber <= _expectedPackageNumber)
        {
            // prepare the response buffer
            _currentOutBuffer = _bufferPool.GetAndOccupyFreeBuffer();
            _currentSettings.outConfirmationNumber = inNumber;
            _currentSettings.needConfirmation = false;

            if(inNumber == _expectedPackageNumber)
            {
                _expectedPackageNumber++;
            }
            return buffer.size() > 0;
        }
        return false; // discard the packet and wait for the lost ones
    }

    void UdpPacketDeliverer::ConfirmPacket(uint32_t number)
    {
        for(auto it = _unconfirmedPackets.begin(); it != _unconfirmedPackets.end(); ++it)
            if(it->first == number)
            {
                _unconfirmedPackets.erase(it);
                break;
            }
    }

    boost::beast::flat_buffer& UdpPacketDeliverer::GetOutBuffer()
    {
        if(!_currentOutBuffer)
            _currentOutBuffer = _bufferPool.GetAndOccupyFreeBuffer();
        return _currentOutBuffer->body;
    }

    void UdpPacketDeliverer::SetConfirmOfCurrentPackage(bool confirm)
    {
        _currentSettings.needConfirmation |= confirm;
    }

    void UdpPacketDeliverer::SendCurrentData()
    {
        if(!_currentOutBuffer || _currentOutBuffer->body.size() == 0) return;

        if(_currentSettings.needConfirmation)
        {
            _currentOutBuffer->autoFree = false;
            Utils::WriteToBuffer(_currentOutBuffer->header, _nextPackageNumber);
            _unconfirmedPackets.emplace_back(_nextPackageNumber, _currentOutBuffer);
            _nextPackageNumber++;
        }
        else
        {
            Utils::WriteToBuffer(_currentOutBuffer->header, uint32_t{0});
        }
        if(_currentSettings.outConfirmationNumber > 0)
        {
            Utils::WriteToBuffer(_currentOutBuffer->header, uint8_t{1});
            Utils::WriteToBuffer(_currentOutBuffer->header, _currentSettings.outConfirmationNumber);
        }
        else
        {
            Utils::WriteToBuffer(_currentOutBuffer->header, uint8_t{0});
        }
        _udpMaster.SendData(_currentOutBuffer, _endpoint);
        ResetSelfState();
    }

    void UdpPacketDeliverer::ResetSelfState()
    {
        _currentOutBuffer.reset();
        _currentSettings.needConfirmation = false;
        _currentSettings.outConfirmationNumber = 0;
    }

    void UdpPacketDeliverer::RepeatSend()
    {
        if(_unconfirmedPackets.size() > 0)
        {
            for(auto& item : _unconfirmedPackets)
            {
                if(item.second->Sent())
                {
                    item.second->Sent(false);
                    _udpMaster.SendData(item.second, _endpoint);
                }
            }
        }
    }
}
}