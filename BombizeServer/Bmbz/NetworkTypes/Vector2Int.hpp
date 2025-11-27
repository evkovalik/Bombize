#ifndef BMBZ_NETTYPE_VECTOR2INT_HPP
#define BMBZ_NETTYPE_VECTOR2INT_HPP

#include <cstdint>
#include "Vector2.hpp"

namespace Bmbz
{
namespace NetType
{
    struct Vector2Int : public Vector2<int>
    {
        Vector2Int() : Vector2<int>(0) {}
        Vector2Int(int x, int y) : Vector2<int>(x, y) {}

        static Vector2Int Zero()    { return Vector2Int{0, 0}; }
        static Vector2Int Left()    { return Vector2Int{-1, 0}; }
        static Vector2Int Right()   { return Vector2Int{1, 0}; }
        static Vector2Int Up()      { return Vector2Int{0, 1}; }
        static Vector2Int Down()    { return Vector2Int{0, -1}; }

        friend Vector2Int operator+(const Vector2Int& lhs, const Vector2Int& rhs)
        {
            return Vector2Int{lhs.x + rhs.x, lhs.y + rhs.y};
        }

        friend Vector2Int operator-(const Vector2Int& lhs, const Vector2Int& rhs)
        {
            return Vector2Int{lhs.x - rhs.x, lhs.y - rhs.y};
        }
    };
}
}
#endif