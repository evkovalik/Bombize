#ifndef BMBZ_GAME_COMPONENT_MAPDESTROYER_HPP
#define BMBZ_GAME_COMPONENT_MAPDESTROYER_HPP

#include <optional>
#include "NetworkTypes/Vector2Int.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
    class MapDestroyer
    {
    public:
        void Init(int sizeX, int sizeY, int border=0)
        {
            _countSteps = (sizeX - border * 2) * (sizeY - border * 2);
            _sizeOX.Value(border, sizeX - border);
            _sizeOY.Value(border, sizeY - border);
            _pos.Value(border, border - 1);
        }

        std::optional<NetType::Vector2Int> GetNextDestroyCell()
        {
            if(_countSteps <= 0) return std::nullopt;

            NetType::Vector2Int nextPos = _pos + _dirVec;

            if(nextPos.y >= _sizeOY.y) // out up
            {
                _dirVec = NetType::Vector2Int::Right();
                _sizeOX.x++;
                return GetNextDestroyCell();
            }
            else if(nextPos.x >= _sizeOX.y) // out right
            {
                _dirVec = NetType::Vector2Int::Down();
                _sizeOY.y--;
                return GetNextDestroyCell();
            }
            else if(nextPos.y < _sizeOY.x) // out down
            {
                _dirVec = NetType::Vector2Int::Left();
                _sizeOX.y--;
                return GetNextDestroyCell();
            }
            else if(nextPos.x < _sizeOX.x) // out left
            {
                _dirVec = NetType::Vector2Int::Up();
                _sizeOY.x++;
                return GetNextDestroyCell();
            }
            else
            {
                _pos = nextPos;
                _countSteps--;
                return _pos;
            }
        }
        
    private:
        int _countSteps{0};
        NetType::Vector2Int _sizeOX;
        NetType::Vector2Int _sizeOY;
        NetType::Vector2Int _pos{0, -1};
        NetType::Vector2Int _dirVec = NetType::Vector2Int::Up();
    };
}
}
}
#endif