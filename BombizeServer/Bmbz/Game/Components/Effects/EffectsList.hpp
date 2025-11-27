#ifndef BMBZ_GAME_COMPONENT_EFFECT_EFFECTSLIST_HPP
#define BMBZ_GAME_COMPONENT_EFFECT_EFFECTSLIST_HPP

#include "Game/Components/Effects/IEffect.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
namespace Effect
{
    class EffectsList final : public IEffect
    {
    public:
        template<typename... Effects>
        EffectsList(Effects&&... effects)
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
            if(!_isActive) return;
            
            _isActive = false;
            for(auto& effect : _effects)
            {
                if(effect->IsActive())
                {
                    effect->Execute(playerInfo);
                    _isActive = true;
                }
            }
        }
        bool IsActive() const override { return _isActive; }

        void Enable() override
        {
            for(auto& effect : _effects)
                effect->Enable();
            _isActive = true;
        }
        void Disable() override
        {
            for(auto& effect : _effects)
                effect->Disable();
            _isActive = false;
        }
    private:
        bool _isActive = true;
        std::vector<std::unique_ptr<IEffect>> _effects;
    };
}
}
}
}
#endif