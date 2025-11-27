#ifndef BMBZ_GAME_COMPONENT_EFFECT_EFFECTSSEQUENCE_HPP
#define BMBZ_GAME_COMPONENT_EFFECT_EFFECTSSEQUENCE_HPP

#include "Game/Components/Effects/IEffect.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
namespace Effect
{
    class EffectsSequence final : public IEffect
    {
    public:
        template<typename... Effects>
        EffectsSequence(Effects&&... effects)
        {
            static_assert(
                (std::is_convertible_v<Effects, std::unique_ptr<IEffect>> && ...),
                "All arguments must be exactly std::unique_ptr<IEffect>"
            );
            _effects.reserve(sizeof...(effects));
            ((_effects.push_back(std::forward<Effects>(effects))), ...);
        }

        void Execute(NetworkData::PlayerInfo& playerInfo) override
        {
            if(!_isActive || _currentIndex >= _effects.size()) return;

            _effects[_currentIndex]->Execute(playerInfo);
            if(!_effects[_currentIndex]->IsActive())
            {
                _currentIndex++;
                Execute(playerInfo);
            }
        }
        bool IsActive() const override { return _isActive; }

        void Enable() override
        {
            if(_currentIndex < _effects.size())
                _effects[_currentIndex]->Enable();
            _isActive = true;
        }
        void Disable() override
        {
            if(_currentIndex < _effects.size())
                _effects[_currentIndex]->Disable();
            _isActive = false;
        }
    private:
        bool _isActive = true;
        std::vector<std::unique_ptr<IEffect>> _effects;
        size_t _currentIndex = 0;
    };
}
}
}
}
#endif