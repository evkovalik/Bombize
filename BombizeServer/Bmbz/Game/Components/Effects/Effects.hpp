#ifndef BMBZ_GAME_COMPONENT_EFFECTS_HPP
#define BMBZ_GAME_COMPONENT_EFFECTS_HPP

#include "EmptyEffect.hpp"
#include "Changes/ChangeHealth.hpp"
#include "Changes/ChangeBomb.hpp"
#include "Changes/ChangeFire.hpp"
#include "Changes/ChangeSpeed.hpp"
#include "TemporaryChange.hpp"
#include "EffectsList.hpp"
#include "EffectsSequence.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
namespace Effect
{
    static std::unique_ptr<IEffect> TempBomb(int diffSize, int timeMS)
    {
        return std::make_unique<Game::Component::Effect::TemporaryChange>(
            std::make_unique<Game::Component::Effect::ChangeBomb>(diffSize),
            std::chrono::milliseconds(timeMS)
        );
    }

    static std::unique_ptr<IEffect> TempSpeed(int diffSize, int timeMS)
    {
        return std::make_unique<Game::Component::Effect::TemporaryChange>(
            std::make_unique<Game::Component::Effect::ChangeSpeed>(diffSize),
            std::chrono::milliseconds(timeMS)
        );
    }

    static std::unique_ptr<IEffect> TempFire(int diffSize, int timeMS)
    {
        return std::make_unique<Game::Component::Effect::TemporaryChange>(
            std::make_unique<Game::Component::Effect::ChangeFire>(diffSize),
            std::chrono::milliseconds(timeMS)
        );
    }
}
}
}
}
#endif