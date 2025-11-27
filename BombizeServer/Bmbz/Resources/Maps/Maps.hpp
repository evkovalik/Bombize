#ifndef BMBZ_RESOURCES_MAPS_HPP
#define BMBZ_RESOURCES_MAPS_HPP

#include <filesystem>
#include <fstream>
#include <boost/json.hpp>
#include <iostream>
#include "MapInfo.hpp"

namespace Bmbz
{
namespace Resource
{
    class Maps
    {
    public:
        Maps() = delete;
        Maps(std::string mapStorageFolderPath)
        : _dir(std::move(mapStorageFolderPath))
        {}

        MapInfo GetMapInfo(int index) const
        {
            if(!_isMapsData)
                throw std::runtime_error("Maps data has not been uploaded");
            if(index >= _rawMapsData.size())
                throw std::invalid_argument("The \"Maps::index\" argument is out of range");

            MapInfo info;
            info.width = static_cast<int>(_rawMapsData[index].at("width").as_int64());
            info.height = static_cast<int>(_rawMapsData[index].at("height").as_int64());
            info.background = static_cast<int>(_rawMapsData[index].at("background").as_int64());

            info.field.reserve(_rawMapsData[index].at("field").as_array().size());
            for(auto& value : _rawMapsData[index].at("field").as_array())
                info.field.push_back(static_cast<uint8_t>(value.as_int64()));

            info.spawnPositions.reserve(_rawMapsData[index].at("spawnPositions").as_array().size());
            for(auto& pos : _rawMapsData[index].at("spawnPositions").as_array())
            {
                NetType::Vector2<uint8_t> spawnPos{
                    static_cast<uint8_t>(pos.at("x").as_int64()),
                    static_cast<uint8_t>(pos.at("y").as_int64()),
                };
                info.spawnPositions.push_back(std::move(spawnPos));
            }
            info.timeLife = std::chrono::seconds(_rawMapsData[index].at("timeLifeSec").as_int64());
            info.rateDestruction = std::chrono::milliseconds(_rawMapsData[index].at("rateDestructionMS").as_int64());
            return info;
        }

        auto Count() const noexcept { return _rawMapsData.size(); }

        void LoadAll()
        {
            CheckDir();
            Load();
        }

    private:
        std::string _dir;
        bool _isMapsData = false;
        std::vector<std::string> _namesOfDataFiles;
        std::vector<boost::json::value> _rawMapsData;

        void CheckDir()
        {
            try
            {
                for (const auto& entry : std::filesystem::directory_iterator(_dir))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".json")
                    {
                        std::cout << entry.path().string() << std::endl;
                        _namesOfDataFiles.push_back(entry.path().string());
                    }
                }
            }
            catch (const std::filesystem::filesystem_error& e)
            {
                std::cerr << "Directory access error: " << e.what() << std::endl;
            }
        }

        void Load()
        {
            try
            {
                for(const auto& fileName : _namesOfDataFiles)
                {
                    std::ifstream fileStream(fileName);
                    if(!fileStream.is_open())
                        throw std::runtime_error("Couldn't open the file: " + fileName);

                    boost::json::stream_parser parser;
                    std::string line;
                    while(std::getline(fileStream, line))
                    {
                        parser.write(line);
                    }
                    _rawMapsData.push_back(parser.release());
                }
                _isMapsData = true;
            }
            catch(const std::exception& e)
            {
                std::cout << "Error loading maps: " << e.what() << '\n';
            }
        }
    };
}
}
#endif