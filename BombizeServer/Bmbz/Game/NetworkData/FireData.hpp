#ifndef BMBZ_GAME_FIREDATA_HPP
#define BMBZ_GAME_FIREDATA_HPP

#include "Utils/ReadWriteData.hpp"
#include "NetworkTypes/Vector2.hpp"

namespace Bmbz
{
namespace Game
{
namespace NetworkData
{
    struct FireData
    {
        NetType::Vector2<uint8_t> center;

        bool
        destructionOnLeft = false,
        destructionOnRight = false,
        destructionOnUp = false,
        destructionOnDown = false;
        
        uint8_t
        lengthLeft = 0,
        lengthRight = 0,
        lengthUp = 0,
        lengthDown = 0;

        std::array<std::optional<NetType::Vector2<uint8_t>>, 4> GetDestructions()
        {
            std::array<std::optional<NetType::Vector2<uint8_t>>, 4> sides; // Left, Up, Right, Down
            if(destructionOnLeft)
                sides[0].emplace(center.x - lengthLeft, center.y);
            if(destructionOnUp)
                sides[1].emplace(center.x, center.y + lengthUp);
            if(destructionOnRight)
                sides[2].emplace(center.x + lengthRight, center.y);
            if(destructionOnDown)
                sides[3].emplace(center.x, center.y - lengthDown);
            return sides;
        }

        void Serialize(boost::beast::flat_buffer& buffer) const
        {
            Utils::WriteToBuffer(buffer, lengthLeft, lengthRight, lengthUp, lengthDown);
        }
    };
}
}
}
#endif