#include "Map.hpp"
#include "Avatar.hpp"
#include <array>

namespace Bmbz
{
namespace Game
{
    using CellType = Resource::MapInfo::CellType;

    static constexpr std::array<CellType, 6> DESTRUCTIBLE_CELLS // temp. add load from file
    {
        8,9,10,11,12,13
    };
    static constexpr CellType EMPTY_CELL = 0;
    static constexpr CellType OCCUPIED_CELL = -1;
    static constexpr CellType SPAWN_CELL = 1; // temp. add to map.json

    void Map::Init(Resource::MapInfo&& mapInfo)
    {
        _mapInfo = std::move(mapInfo);
        _mapDestroyer.Init(_mapInfo.width, _mapInfo.height);
        _waitingTimerId = Context().Time().AsyncWait(_mapInfo.timeLife, [this](){ DestructionStep(); });
    }

    void Map::OnDestroy()
    {
        if(_waitingTimerId.has_value())
            Context().Time().ResetAsyncWait(_waitingTimerId.value());
    }

    bool Map::TrySetBomb(uint8_t x, uint8_t y)
    {
        if(GetCellValue(x, y) == EMPTY_CELL)
        {
            SetCellValue(x, y, OCCUPIED_CELL);
            return true;
        }
        else return false;
    }

    void Map::RemoveBomb(uint8_t x, uint8_t y)
    {
        if(GetCellValue(x, y) == OCCUPIED_CELL)
            SetCellValue(x, y, EMPTY_CELL);
    }

    NetworkData::FireData Map::ApplyBombFire(uint8_t x, uint8_t y, uint8_t lengthFire)
    {
        NetworkData::FireData fireData;
        fireData.center = NetType::Vector2<uint8_t>{x, y};
        NetType::Vector2<int> startPositionInt{static_cast<int>(x), static_cast<int>(y)};

        auto fireInDirection = 
        [&](const NetType::Vector2<int>& direction, uint8_t& length, bool& isDestruction) -> void
        {
            length = 0;
            isDestruction = false;
            auto cellPos = startPositionInt;
            for(length = 0; length < lengthFire; ++length)
            {
                cellPos += direction;

                if(cellPos.x < 0 || cellPos.x >= _mapInfo.width 
                || cellPos.y < 0 || cellPos.y >= _mapInfo.height)
                {
                    return;
                }

                auto cellValues = GetCellValue(cellPos.x, cellPos.y);

                if(cellValues == EMPTY_CELL || cellValues == OCCUPIED_CELL)
                    continue;

                auto it = std::find(DESTRUCTIBLE_CELLS.begin(), DESTRUCTIBLE_CELLS.end(), cellValues);       
                if(it != DESTRUCTIBLE_CELLS.end())
                {
                    SetCellValue(cellPos.x, cellPos.y, EMPTY_CELL);
                    Context().Writer().ReplaceMapCell(
                        static_cast<uint8_t>(cellPos.x),
                        static_cast<uint8_t>(cellPos.y),
                        EMPTY_CELL
                    );
                    isDestruction = true;
                    length += 1;
                    return;
                }
                else return; // indestructible cell
            }
            return;
        };
        fireInDirection(
            NetType::Vector2<int>(-1, 0), fireData.lengthLeft, fireData.destructionOnLeft
        );
        fireInDirection(
            NetType::Vector2<int>(1, 0), fireData.lengthRight, fireData.destructionOnRight
        );
        fireInDirection(
            NetType::Vector2<int>(0, 1), fireData.lengthUp, fireData.destructionOnUp
        );
        fireInDirection(
            NetType::Vector2<int>(0, -1), fireData.lengthDown, fireData.destructionOnDown
        );
        return std::move(fireData);
    }

    void Map::DestructionStep()
    {
        auto result = _mapDestroyer.GetNextDestroyCell();
        if(result.has_value())
        {
            auto cellPos = NetType::Vector2<uint8_t>{
                static_cast<uint8_t>(result.value().x),
                static_cast<uint8_t>(result.value().y)
            };
            SetCellValue(cellPos.x, cellPos.y, SPAWN_CELL);
            Context().Writer().ReplaceMapCell(cellPos.x, cellPos.y, SPAWN_CELL);
            
            _checkBox.SetPosition(cellPos);
            if(auto other = Context().Collisions().CheckWithAll(&_checkBox))
            {
                if(other->Owner().Tag() == ObjectTag::AVATAR)
                {
                    auto& avatar = static_cast<Avatar&>(other->Owner());
                    avatar.Kill();
                }
            }
            _waitingTimerId = Context().Time().AsyncWait(
                _mapInfo.rateDestruction, [this](){ DestructionStep(); }
            );
        }
        else
        {
            // Everything is ruined
            //Context().Objects().DeleteObject(this);
        }
    }
}
}