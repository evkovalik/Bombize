#ifndef BMBZ_RESOURCES_RESOURCELOCATOR_HPP
#define BMBZ_RESOURCES_RESOURCELOCATOR_HPP

#include <memory>
#include "Maps/Maps.hpp"
#include "Characters/Characters.hpp"

namespace Bmbz
{
    class ResourceLocator
    {
    public:
        static const Resource::Maps& GetMaps()
        {
            if(!_maps) throw std::runtime_error("The resource (Maps) was not provided");
            return *(_maps.get());
        }

        static const Resource::Characters& GetCharacters()
        {
            if(!_characters) throw std::runtime_error("The resource (Characters) was not provided");
            return *(_characters.get());
        }
        
        static void Provide(std::unique_ptr<Resource::Maps>&& maps)
        {
            _maps = std::move(maps);
        }

        static void Provide(std::unique_ptr<Resource::Characters>&& characters)
        {
            _characters = std::move(characters);
        }

    private:
        inline static std::unique_ptr<Resource::Maps> _maps = nullptr;
        inline static std::unique_ptr<Resource::Characters> _characters = nullptr;

    };
}
#endif