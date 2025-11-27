#ifndef BMBZ_PREFAB_CELLTYPE_HPP
#define BMBZ_PREFAB_CELLTYPE_HPP

#include <cstdint>

namespace Bmbz
{
	namespace Prefab
	{
        // cell bits:
        enum CellMask : uint8_t
        {
            Occupied = 1,       // wall, bomb
            Destructible = 2,   // wall
            LetThrough = 4      // bomb
        };
    }
}
#endif