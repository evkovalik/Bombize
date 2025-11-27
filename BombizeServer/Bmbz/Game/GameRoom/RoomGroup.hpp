#ifndef BMBZ_GAME_ROOMGROUP_HPP
#define BMBZ_GAME_ROOMGROUP_HPP

#include "Network/UdpMaster.hpp"
#include "Room.hpp"
#include "Game/Structs/GroupToGame.hpp"

namespace Bmbz
{
namespace Game
{
    class RoomGroup
    {
    public:
        explicit RoomGroup(
            boost::asio::io_context& ioc,
            boost::asio::ip::udp::endpoint endpoint
        )
        : _strand(boost::asio::make_strand(ioc.get_executor()))
        , _udpMaster(_strand, std::move(endpoint),
            [this]
            (boost::beast::flat_buffer& buffer, const boost::asio::ip::udp::endpoint& remoteEndpoint)
            { OnInputMessage(buffer, remoteEndpoint); })
        {}

        ~RoomGroup() = default;
        RoomGroup(const RoomGroup&) = delete;
        RoomGroup& operator=(const RoomGroup& other) = delete;
        RoomGroup(RoomGroup&&) = delete;
        RoomGroup& operator=(RoomGroup&& other) = delete;

        size_t Size() const { return _rooms.size(); }
        const auto& Strand() const { return _strand; }
        const auto& UdpMaster() const { return _udpMaster; }
        const auto& Endpoint() const { return _udpMaster.Endpoint(); }

        void CreateAndRunRoom(GroupToGame gamers)
        {
            auto [it, inserted] = _rooms.emplace(
                _nextRoomNumber, std::make_unique<Room>(*this, _nextRoomNumber)
            );
            _nextRoomNumber++;

            if(inserted)
            {
                it->second->Run(
                    std::move(gamers),
                    [this](int roomNumber){ OnFinishGame(roomNumber); }
                );
            }
        }

    private:
        boost::asio::strand<boost::asio::any_io_executor> _strand;
        Network::UdpMaster _udpMaster;

        std::unordered_map<uint16_t, std::unique_ptr<Room>> _rooms;
        uint16_t _nextRoomNumber = 0;

        void OnInputMessage(
            boost::beast::flat_buffer& inBuffer,
            const boost::asio::ip::udp::endpoint& remoteEndpoint)
        {
            size_t inSize = inBuffer.size();

            uint16_t roomNumber;
            Utils::ReadFromBuffer(inBuffer, roomNumber);
            if(_rooms.contains(roomNumber))
                _rooms[roomNumber]->ProcessingInputData(inBuffer, remoteEndpoint);
            
            // std::cout << "[RoomGroup::OnInputMessage]: InBuffer.size() = " << inSize 
            // << ". Processed = " << inSize - inBuffer.size() << "\n";
        }

        void OnFinishGame(int roomNumber)
        {
            if(_rooms.contains(roomNumber)) _rooms.extract(roomNumber);
            std::cout << "Count rooms = " << _rooms.size() << std::endl;
        }
    };
}
}
#endif