#ifndef BMBZ_RESOURCES_CHARACTERS_HPP
#define BMBZ_RESOURCES_CHARACTERS_HPP

#include <array>
#include <stdexcept>
#include "CharacterInfo.hpp"
#include "Game/Components/Effects/Effects.hpp"

namespace Bmbz
{
namespace Resource
{
    class Characters
    {
    public:
        const CharacterInfo& GetCharacterInfo(uint32_t index) const
        {
            if(index >= _storage.size())
                throw std::runtime_error("Characters::GetCharacterInfo: Out of range");
            return _storage[index];
        }
        size_t Count() const { return _storage.size(); }
    private:

        std::array<CharacterInfo, 4> _storage
        {
            CharacterInfo{ // default
                0,
                0,
                [](){return std::make_unique<Game::Component::Effect::EmptyEffect>();},
                [](){return std::make_unique<Game::Component::Effect::EmptyEffect>();}
            },
            CharacterInfo{ // ice
                1200,
                12,
                [](){return std::make_unique<Game::Component::Effect::ChangeSpeed>(1);},
                [](){return std::make_unique<Game::Component::Effect::EffectsList>(
                    Game::Component::Effect::TempSpeed(-10, 3000),
                    std::make_unique<Game::Component::Effect::ChangeSpeed>(-2)
                );}
            },
            CharacterInfo{ // fire
                2800,
                28,
                [](){return std::make_unique<Game::Component::Effect::EffectsList>(
                    std::make_unique<Game::Component::Effect::ChangeBomb>(2),
                    std::make_unique<Game::Component::Effect::ChangeFire>(2)
                );},
                [](){return std::make_unique<Game::Component::Effect::EffectsList>(
                    Game::Component::Effect::TempBomb(-5, 8000),
                    Game::Component::Effect::TempFire(-5, 8000)
                );}
            },
            CharacterInfo{ // bot
                3200,
                32,
                [](){return std::make_unique<Game::Component::Effect::EffectsList>(
                    std::make_unique<Game::Component::Effect::ChangeBomb>(1),
                    std::make_unique<Game::Component::Effect::ChangeSpeed>(1)
                );},
                [](){return std::make_unique<Game::Component::Effect::EffectsSequence>(
                    std::make_unique<Game::Component::Effect::EffectsList>(
                        Game::Component::Effect::TempBomb(-10, 5000),
                        Game::Component::Effect::TempFire(-10, 5000)
                    ),
                    std::make_unique<Game::Component::Effect::ChangeBomb>(-1),
                    std::make_unique<Game::Component::Effect::ChangeFire>(-2)
                );}
            }
        };
    };
}
}
#endif