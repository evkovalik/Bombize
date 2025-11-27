#ifndef BMBZ_NETTYPE_INETWORKDATA_HPP
#define BMBZ_NETTYPE_INETWORKDATA_HPP

#include "Utils/ReadWriteData.hpp"

namespace Bmbz
{
namespace NetType
{
    struct IDataForClient
    {
        virtual ~IDataForClient() = default;
        virtual void Serialize(boost::beast::flat_buffer& buffer) const = 0;
    };

    struct IDataFromClient
    {
        virtual ~IDataFromClient() = default;
        virtual void Deserialize(boost::beast::flat_buffer& buffer) = 0;
    };

    struct INetworkData : IDataForClient, IDataFromClient
    {
        virtual ~INetworkData() = default;
    };
}
}
#endif