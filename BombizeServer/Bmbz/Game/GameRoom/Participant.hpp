#ifndef BMBZ_GAME_PARTICIPANT_HPP
#define BMBZ_GAME_PARTICIPANT_HPP

#include "Network/UdpPacketDeliverer.hpp"
#include "Game/GameRoom/InputController/InputController.hpp"
#include "Game/GameObjects/Avatar.hpp"

namespace Bmbz
{
    class UserConnection;
namespace Game
{
    class Participant
    {
    public:
        Participant(
            int number,
            std::shared_ptr<UserConnection> userConnection,
            const Network::UdpMaster& udpMaster
        );

        bool IsConnectionUDP() const { return _deliverer.IsConnection(); }
        auto GetUserConnection() const { return _userConnection; }

        void ProcessInputMessage(
            boost::beast::flat_buffer& inBuffer,
            const boost::asio::ip::udp::endpoint& remoteEndpoint
        );
        void SendActions(const boost::beast::flat_buffer& bufferWithActions, bool confirm);
        void SendOldActions();

        using OnAvatarDeath = std::function<void(Participant&)>;
        void SetAvatar(Avatar* avatar, OnAvatarDeath handler);
        bool HasAvatar() const noexcept { return _avatar != nullptr; }
        void KillAvatar();
    
    private:
        int _number;
        std::shared_ptr<UserConnection> _userConnection;
        Network::UdpPacketDeliverer _deliverer;
        InputController _inputController;
        Avatar* _avatar = nullptr;
    };
}
}
#endif