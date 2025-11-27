#ifndef BMBZ_RESOURCES_CHARACTERINFO_HPP
#define BMBZ_RESOURCES_CHARACTERINFO_HPP

#include <cstdint>
#include <vector>
#include "Game/Components/Effects/IEffect.hpp"

namespace Bmbz
{
namespace Resource
{
    struct CharacterInfo
    {
        using EffectFactory = std::function<std::unique_ptr<Game::Component::IEffect>()>;

        CharacterInfo(
            uint32_t price1,
            uint32_t price2,
            EffectFactory protectionEffectFactory,
            EffectFactory attackEffectFactory
        )
        : price1(price1)
        , price2(price2)
        , _protectionEffect(protectionEffectFactory)
        , _attackEffect(attackEffectFactory)
        {}

        uint32_t price1;
        uint32_t price2;

        std::unique_ptr<Game::Component::IEffect> GetProtectionEffect() const
        {
            return _protectionEffect();
        }

        std::unique_ptr<Game::Component::IEffect> GetAttackEffect() const
        {
            return _attackEffect();
        }

    private:
        EffectFactory _protectionEffect;
        EffectFactory _attackEffect;
    };
}
}
#endif