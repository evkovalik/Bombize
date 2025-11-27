#include "Participant.hpp"
#include "Server/UserConnection.hpp"
#include "Resources/ResourceLocator.hpp"

namespace Bmbz
{
namespace Game
{
    Participant::Participant(
        int number,
        std::shared_ptr<UserConnection> userConnection,
        const Network::UdpMaster& udpMaster
    )
    : _number(number)
    , _userConnection(userConnection)
    , _deliverer(udpMaster)
    {}

    void Participant::ProcessInputMessage(
        boost::beast::flat_buffer& inBuffer,
        const boost::asio::ip::udp::endpoint& remoteEndpoint)
    {
        _deliverer.UpdateEndpoint(remoteEndpoint);
        try
        {
            if(_deliverer.ProcessingInputPacket(inBuffer))
            {
                while(inBuffer.size() > 0)
                {
                    bool success = _inputController.ProcessInputData(inBuffer);
                    if(!success)
                    {
                        std::cout << "unregistered type of game action\n";
                    }
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cout << "Error: Participant::ProcessInputMessage\n";
            std::cerr << e.what() << '\n';
        }
        catch(...)
        {
            std::cerr << "Unknown error in: Participant::ProcessInputMessage" << std::endl;
        }
    }

    void Participant::SendActions(const boost::beast::flat_buffer& bufferWithActions, bool confirm)
    {
        auto& outBuffer = _deliverer.GetOutBuffer();
        auto sendSize = boost::asio::buffer_copy(
            outBuffer.prepare(bufferWithActions.size()), bufferWithActions.cdata()
        );
        outBuffer.commit(sendSize);
        _deliverer.SetConfirmOfCurrentPackage(confirm);

        _deliverer.RepeatSend();
        _deliverer.SendCurrentData();
    }

    void Participant::SendOldActions()
    {
        _deliverer.RepeatSend();
    }

    void Participant::SetAvatar(Avatar* avatar, OnAvatarDeath handler)
    {
        _avatar = avatar;
        auto userInfo = _userConnection->GetUserInfo();
        auto characterId = userInfo->characters[userInfo->activeCharacterIndex].id;

        _avatar->Init(
            _number,
            &ResourceLocator::GetCharacters().GetCharacterInfo(characterId),
            [this, handler = std::move(handler)]()
            {
                _inputController.RemoveListener(_avatar);
                _avatar = nullptr;
                std::move(handler)(*this);
            }
        );
        _inputController.AddListener(_avatar);
    }

    void Participant::KillAvatar()
    {
        if(_avatar)
        {
            _inputController.RemoveListener(_avatar);
            _avatar->Kill(false);
            _avatar = nullptr;
        }
    }
}
}