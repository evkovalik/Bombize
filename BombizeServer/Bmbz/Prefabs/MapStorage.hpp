#ifndef BMBZ_PREFAB_MAPSTORAGE_HPP
#define BMBZ_PREFAB_MAPSTORAGE_HPP

#include "Map.hpp"
#include <array>
#include <string>

namespace Bmbz
{
	namespace Prefab
	{
        class MapStorage
        {
        public:
            static MapStorage& Instance()
            {
                static MapStorage instance;
                return instance;
            }
            
            MapStorage(const MapStorage& ) = delete;
            MapStorage& operator=(const MapStorage&) = delete;
            
            Map GetById(uint8_t id) const
            {
                return _array[id];
            }
            
        private:
            MapStorage() = default;
            std::string dirPath = "/home/user/Projects/Bombize/Server/GameMaps/";

            std::array<Map, 1> _array =
            {
                Map{std::chrono::minutes(3), dirPath + "Map0.txt"}
            };
        };
    }
}
#endif