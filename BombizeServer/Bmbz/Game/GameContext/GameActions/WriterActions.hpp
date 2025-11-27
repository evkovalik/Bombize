#ifndef BMBZ_GAMEACTION_WRITERACTIONS_HPP
#define BMBZ_GAMEACTION_WRITERACTIONS_HPP

#include "Utils/ReadWriteData.hpp"
#include "ActionType.hpp"
#include "NetworkTypes/Vector2Fixed.hpp"
#include "Game/NetworkData/PlayerInfo.hpp"
#include "Game/NetworkData/PlayerPosition.hpp"
#include "Game/NetworkData/FireData.hpp"
#include "Game/Structs/GameResourceType.hpp"

namespace Bmbz
{
namespace Game
{
    class WriterActions
    {
        // <========== Management ==========> //
    private:
        boost::beast::flat_buffer _writeBuffer;
        bool _needConfirmCurrentActions = false;
    public:
        WriterActions() = default;
        WriterActions(size_t bufferSize) : _writeBuffer{bufferSize} {}

        size_t BufferSize() const { return _writeBuffer.size(); }
        bool ConfirmationFlag() const { return _needConfirmCurrentActions; }
        const boost::beast::flat_buffer& GetBuffer() const { return _writeBuffer; }

        void ClearBuffer()
        {
            _writeBuffer.consume(_writeBuffer.size());
            _needConfirmCurrentActions = false;
        }
        void AddConfirmCurrentActions() { _needConfirmCurrentActions |= true; }

        // <========== Action recording functions ==========> //
    public:
        void CreateAvatar(
            int ownerNumber,
            const NetworkData::PlayerInfo& playerInfo,
            const NetType::Vector2Fixed& spawnCoordinate)
        {
            _needConfirmCurrentActions = true;
            Utils::WriteToBuffer(_writeBuffer, ActionType::CreateAvatar);
            Utils::WriteToBuffer(_writeBuffer, static_cast<uint8_t>(ownerNumber));
            playerInfo.Serialize(_writeBuffer);
            
            NetType::Vector2<uint8_t> spawnPos{
                static_cast<uint8_t>(spawnCoordinate.x.Value()),
                static_cast<uint8_t>(spawnCoordinate.y.Value())
            };
            spawnPos.Serialize(_writeBuffer);
        }

        void UpdateAvatarPosition(int ownerNumber, const NetworkData::PlayerPosition& position)
        {
            Utils::WriteToBuffer(_writeBuffer, ActionType::UpdatePosition);
            Utils::WriteToBuffer(_writeBuffer, static_cast<uint8_t>(ownerNumber));
            position.Serialize(_writeBuffer);
        }

        void SetBomb(uint16_t netObjId, int ownerNumber, const NetType::Vector2<uint8_t>& position)
        {
            Utils::WriteToBuffer(_writeBuffer, ActionType::SetBomb);
            Utils::WriteToBuffer(_writeBuffer, netObjId);
            Utils::WriteToBuffer(_writeBuffer, static_cast<uint8_t>(ownerNumber));
            position.Serialize(_writeBuffer);
        }

        void SetFire(
            uint16_t netObjId,
            int ownerNumber,
            const NetType::Vector2<uint8_t>& position,
            const NetworkData::FireData& fireData)
        {
            Utils::WriteToBuffer(_writeBuffer, ActionType::SetFire);
            Utils::WriteToBuffer(_writeBuffer, netObjId);
            Utils::WriteToBuffer(_writeBuffer, static_cast<uint8_t>(ownerNumber));
            position.Serialize(_writeBuffer);
            fireData.Serialize(_writeBuffer);
        }

        void RemoveNetObject(uint16_t netObjId)
        {
            Utils::WriteToBuffer(_writeBuffer, ActionType::RemoveObject);
            Utils::WriteToBuffer(_writeBuffer, netObjId);
        }

        void UpdatePlayerInfo(int ownerNumber, const NetworkData::PlayerInfo& info)
        {
            Utils::WriteToBuffer(_writeBuffer, ActionType::UpdatePlayerInfo);
            Utils::WriteToBuffer(_writeBuffer, static_cast<uint8_t>(ownerNumber));
            info.Serialize(_writeBuffer);
        }

        void ReplaceMapCell(uint8_t cellPosX, uint8_t cellPosY, uint8_t value)
        {
            Utils::WriteToBuffer(_writeBuffer, ActionType::ReplaceCell);
            Utils::WriteToBuffer(_writeBuffer, cellPosX);
            Utils::WriteToBuffer(_writeBuffer, cellPosY);
            Utils::WriteToBuffer(_writeBuffer, value);
        }

        void CreateResource(
            uint16_t netObjId,
            const NetType::Vector2<uint8_t>& position,
            GameResourceType resourceType)
        {
            Utils::WriteToBuffer(_writeBuffer, ActionType::CreateResource);
            Utils::WriteToBuffer(_writeBuffer, netObjId);
            position.Serialize(_writeBuffer);
            Utils::WriteToBuffer(_writeBuffer, static_cast<uint8_t>(resourceType));
        }

        // void PlayerDeath(Game::Player* player)
        // {
        //     Utils::WriteToBuffer(_writeBuffer, ActionType::PlayerDeath);
        //     Utils::WriteToBuffer(_writeBuffer, player->Number());
        // }
};
}
}
#endif