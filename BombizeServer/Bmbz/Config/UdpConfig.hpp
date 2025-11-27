#ifndef BMBZ_CONFIG_UDPCONFIG_HPP
#define BMBZ_CONFIG_UDPCONFIG_HPP

#include <string>

namespace Bmbz
{
namespace Config
{
    struct UdpConfig
    {
        const std::string host;
        const unsigned portStart;
        const unsigned portCount;
        const unsigned maxRoomsPerPort;
    };
}
}
#endif