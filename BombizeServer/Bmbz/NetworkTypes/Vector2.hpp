#ifndef BMBZ_NETTYPE_VECTOR2_HPP
#define BMBZ_NETTYPE_VECTOR2_HPP

#include <cstdint>
#include "INetworkData.hpp"

namespace Bmbz
{
namespace NetType
{
    template<typename T>
    struct Vector2 : public INetworkData
    {
        T x;
        T y;
        Vector2() = default;
        Vector2(const T& xy) : x(xy), y(xy) {}
        template<typename U, typename V>
        Vector2(U&& x, V&& y) : x(std::forward<U>(x)), y(std::forward<V>(y)) {}
        
        virtual ~Vector2() = default;
        
        void Value(T setX, T setY)
        {
            x = setX;
            y = setY;
        }

        Vector2<T>& operator+=(const Vector2<T>& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        bool operator==(const Vector2<T>& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        Vector2<T>& operator+=(const decltype(x)& rhs)
        {
            x += rhs;
            y += rhs;
            return *this;
        }

        Vector2<T>& operator=(const decltype(x)& rhs)
        {
            x = rhs;
            y = rhs;
            return *this;
        }

        friend Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs)
        {
            return Vector2<T>{lhs.x + rhs.x, lhs.y + rhs.y};
        }

        friend Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs)
        {
            return Vector2<T>{lhs.x - rhs.x, lhs.y - rhs.y};
        }

        template<typename U>
        explicit operator Vector2<U>() const
        {
            return Vector2<U>
            {
                static_cast<U>(x),
                static_cast<U>(y)
            };
        }

        void Serialize(boost::beast::flat_buffer& buffer) const override
        {
            if constexpr (std::is_trivially_copyable_v<T>)
            {
                Utils::WriteToBuffer(buffer, x);
                Utils::WriteToBuffer(buffer, y);
            }
            else
            {
                throw "Serialization is not implemented!";
            }
        }

        void Deserialize(boost::beast::flat_buffer& buffer) override
        {
            if constexpr (std::is_trivially_copyable_v<T>)
            {
                Utils::ReadFromBuffer(buffer, x);
                Utils::ReadFromBuffer(buffer, y);
            }
            else
            {
                throw "Deserialization is not implemented!";
            }
        }
    };
}
}
#endif