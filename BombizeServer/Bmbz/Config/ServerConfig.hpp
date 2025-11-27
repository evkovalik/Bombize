#ifndef BMBZ_CONFIG_SERVERCONFIG_HPP
#define BMBZ_CONFIG_SERVERCONFIG_HPP

#include <fstream>
#include <iostream>
#include <boost/json.hpp>
#include "TcpConfig.hpp"
#include "UdpConfig.hpp"
#include "DBConfig.hpp"
#include <iostream>

namespace Bmbz
{
namespace Config
{
    class ServerConfig
    {
    public:
        ServerConfig(std::string configPath)
        {
            Reload(std::move(configPath));
        }

        void Reload(std::string configPath)
        {
            try
            {
                std::ifstream fileStream(configPath);
                if(!fileStream.is_open())
                    throw std::runtime_error("Couldn't open the file: " + configPath);

                boost::json::stream_parser parser;
                std::string line;
                while(std::getline(fileStream, line))
                {
                    parser.write(line);
                }
                _loadData = parser.release();
            }
            catch(const std::exception& e)
            {
                std::cout << "Error loading the server configuration.\n" << e.what() << '\n';
            }
        }

        TcpConfig GetTcpConfig() const
        {
            return TcpConfig
            {
                _loadData.at("tcp_host").as_string().c_str(),
                static_cast<unsigned short>(_loadData.at("tcp_port").as_int64()),
                static_cast<int>(_loadData.at("max_connections").as_int64())
            };
        }

        DBConfig GetDBConfig() const
        {
            return DBConfig
            {
                _loadData.at("db_host").as_string().c_str(),
                static_cast<unsigned>(_loadData.at("db_port").as_int64()),
                _loadData.at("db_login").as_string().c_str(),
                _loadData.at("db_password").as_string().c_str(),
                _loadData.at("db_name_db").as_string().c_str(),
                _loadData.at("db_user_table").as_string().c_str(),
                static_cast<int>(_loadData.at("db_threads").as_int64())
            };
        }

        UdpConfig GetUdpConfig() const
        {
            return UdpConfig
            {
                _loadData.at("udp_host").as_string().c_str(),
                static_cast<unsigned>(_loadData.at("udp_port_start").as_int64()),
                static_cast<unsigned>(_loadData.at("udp_port_count").as_int64()),
                static_cast<unsigned>(_loadData.at("max_rooms_in_group").as_int64())
            };
        }

        std::string GetGameMapsDirPath() const
        {
            return _loadData.at("game_maps_dir").as_string().c_str();
        }

    private:
        boost::json::value _loadData;
    };
}
}
#endif