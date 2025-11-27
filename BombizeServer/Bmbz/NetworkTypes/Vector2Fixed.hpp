#ifndef BMBZ_NETTYPE_VECTOR2FIXED16BIT_HPP
#define BMBZ_NETTYPE_VECTOR2FIXED16BIT_HPP

#include "Vector2.hpp"
#include "FloatFixed.hpp"

namespace Bmbz
{
namespace NetType
{
    struct Vector2Fixed16Bit : public Vector2<FloatFixed16Bit>
    {
        Vector2Fixed16Bit() = delete;
        using Vector2<FloatFixed16Bit>::Vector2;

        void Value(float setX, float setY)
        {
            x.Value(setX);
            y.Value(setY);
        }

        void Serialize(boost::beast::flat_buffer& buffer) const override
        {
            x.Serialize(buffer);
            y.Serialize(buffer);
        }

        void Deserialize(boost::beast::flat_buffer& buffer) override
        {
            x.Deserialize(buffer);
            y.Deserialize(buffer);
        }
    };
    using Vector2Fixed = Vector2Fixed16Bit;
}
}
#endif