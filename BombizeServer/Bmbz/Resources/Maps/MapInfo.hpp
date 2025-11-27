#ifndef BMBZ_RESOURCES_MAPINFO_HPP
#define BMBZ_RESOURCES_MAPINFO_HPP

#include <cstdint>
#include <vector>
#include <chrono>
#include "NetworkTypes/Vector2.hpp"

namespace Bmbz
{
namespace Resource
{
    struct MapInfo
    {
        using CellType = int8_t;
        
        int width;
        int height;
        int background;
        std::vector<NetType::Vector2<uint8_t>> spawnPositions;
        std::vector<uint8_t> field;
        std::chrono::seconds timeLife;
        std::chrono::milliseconds rateDestruction;

        void Serialize(boost::beast::flat_buffer& buffer) const
        {
            Utils::WriteToBuffer(buffer, static_cast<uint8_t>(background));
            Utils::WriteToBuffer(buffer, static_cast<uint8_t>(width));
            Utils::WriteToBuffer(buffer, static_cast<uint8_t>(height));

            for(auto cellValue : field)
                Utils::WriteToBuffer(buffer, static_cast<uint8_t>(cellValue));
        }
    };
}
}
#endif