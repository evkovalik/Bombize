#ifndef BMBZ_GAME_GAMERANDOM_HPP
#define BMBZ_GAME_GAMERANDOM_HPP

#include "Generators/GenWithProbability.hpp"
#include "Game/Structs/GameResourceType.hpp"

namespace Bmbz
{
namespace Game
{
    class GameRandom
    {
    public:
        std::optional<GameResourceType> GetResource()
        {
            auto result = _genResources.TryCreateValue();
            if(result.has_value())
                return static_cast<GameResourceType>(result.value());
            else
                return std::nullopt;
        }

    private:
        static constexpr int PROBABILITY_RESOURCE = 50; // %
        Generator::GenWithProbability _genResources{
            0, static_cast<int>(GameResourceType::MAX) - 1, PROBABILITY_RESOURCE
        };
    
    };
}
}
#endif