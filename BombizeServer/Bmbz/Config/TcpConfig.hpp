#ifndef BMBZ_CONFIG_TCPCONFIG_HPP
#define BMBZ_CONFIG_TCPCONFIG_HPP

#include <string>

namespace Bmbz
{
namespace Config
{
    struct TcpConfig
    {
        const std::string host;
        const unsigned short port;
        const int maxConnections;
    };
}
}
#endif