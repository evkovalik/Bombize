#ifndef BMBZ_NETWORKDATA_PLAYERPOSITION_HPP
#define BMBZ_NETWORKDATA_PLAYERPOSITION_HPP

#include "NetworkTypes/Vector2Fixed.hpp"

namespace Bmbz
{
namespace Game
{
namespace NetworkData
{
    struct PlayerPosition
    {
        NetType::Vector2Fixed coordinate;
        uint32_t lable = 0;

        PlayerPosition()
        : coordinate{NetType::FloatFixed{0.0f, 40.0f, 0.01f}}
        {}

        template<typename T>
        NetType::Vector2<T> GetAsVector2() const
        {
            return NetType::Vector2<T>(
                Utils::RoundTo<T>(coordinate.x.Value()), 
                Utils::RoundTo<T>(coordinate.y.Value())
            );
        }

        void Serialize(boost::beast::flat_buffer& buffer) const 
        {
            coordinate.Serialize(buffer);
            Utils::WriteToBuffer(buffer, lable);
        }

        void Deserialize(boost::beast::flat_buffer& buffer) 
        {
            coordinate.Deserialize(buffer);
            Utils::ReadFromBuffer(buffer, lable);
        }
    };
}
}
}
#endif