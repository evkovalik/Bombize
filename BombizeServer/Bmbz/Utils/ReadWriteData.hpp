#ifndef BMBZ_READWRITEDATA_HPP
#define BMBZ_READWRITEDATA_HPP

#include <type_traits>
#include <iostream>
#include <boost/beast/core/flat_buffer.hpp>

namespace Bmbz
{
namespace Utils
{
    template<typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, void>
    static ReadFromBuffer(boost::beast::flat_buffer& buffer, T& value)
    {
        std::memcpy(&value, buffer.data().data(), sizeof(value));
        buffer.consume(sizeof(value));
    }

    template<typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, void>
    static WriteToBuffer(boost::beast::flat_buffer& buffer, const T& value)
    {
        std::memcpy(buffer.prepare(sizeof(value)).data(), &value, sizeof(value));
        buffer.commit(sizeof(value));
    }

    template<typename... Args>
    std::enable_if_t<(std::is_trivially_copyable_v<Args> && ...), void>
    static WriteToBuffer(boost::beast::flat_buffer& buffer, const Args&... values)
    {
        auto writeOne = [&buffer](const auto& value)
        {
            std::memcpy(buffer.prepare(sizeof(value)).data(), &value, sizeof(value));
            buffer.commit(sizeof(value));
        };
        (writeOne(values), ...);
    }

    // template<typename T>
    // std::enable_if_t<std::is_trivially_copyable_v<T>, void>
    // static WriteToStream(std::ostream& toStream, const T& value)
    // {
    //     toStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
    // }

    // template<typename T>
    // std::enable_if_t<std::is_trivially_copyable_v<T>, void>
    // static ReadFromStream(std::istream& fromStream, T& value)
    // {
    //     fromStream.read(reinterpret_cast<char*>(&value), sizeof(value));
    // }
}
}
#endif