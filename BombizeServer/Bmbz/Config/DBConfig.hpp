#ifndef BMBZ_CONFIG_DBCONFIG_HPP
#define BMBZ_CONFIG_DBCONFIG_HPP

#include <string>

namespace Bmbz
{
namespace Config
{
    struct DBConfig
    {
        const std::string host;
        const unsigned port;
        const std::string login;
        const std::string password;
        const std::string dbName;
        const std::string tableName;
        const int threadsCount;
    };
}
}
#endif