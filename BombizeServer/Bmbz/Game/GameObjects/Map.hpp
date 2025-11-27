#ifndef BMBZ_GAME_MAP_HPP
#define BMBZ_GAME_MAP_HPP

#include <chrono>
#include "Game/GameContext/GameContext.hpp"
#include "Resources/Maps/MapInfo.hpp"
#include "Game/NetworkData/FireData.hpp"
#include "Game/Components/MapDestroyer.hpp"
#include "Game/Components/Colliders/BoxCollider.hpp"

namespace Bmbz
{
namespace Game
{
    class Map : public GameObject
    {
    public:
        void OnDestroy() override;
        void Init(Resource::MapInfo&& mapInfo);
        const auto& GetSpawnPositions() const { return _mapInfo.spawnPositions; }
        bool TrySetBomb(uint8_t x, uint8_t y);
		void RemoveBomb(uint8_t x, uint8_t y);
        NetworkData::FireData ApplyBombFire(uint8_t x, uint8_t y, uint8_t lengthFire);
    private:
        using CellType = Resource::MapInfo::CellType;

        Resource::MapInfo _mapInfo;
        Component::MapDestroyer _mapDestroyer;
        Component::BoxCollider _checkBox{*this};
        std::optional<int> _waitingTimerId = std::nullopt;

        inline CellType GetCellValue(uint8_t x, uint8_t y) const
        {
            return _mapInfo.field[x + y * _mapInfo.width];
        }
        inline void SetCellValue(uint8_t x, uint8_t y, CellType value)
        {
            _mapInfo.field[x + y * _mapInfo.width] = value;
        }

        void DestructionStep();
    };
}
}
#endif