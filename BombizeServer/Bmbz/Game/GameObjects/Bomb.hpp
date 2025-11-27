#ifndef BMBZ_GAME_BOMB_HPP
#define BMBZ_GAME_BOMB_HPP

#include <chrono>
#include "Game/GameContext/GameContext.hpp"
#include "Game/Structs/ExplosionTarget.hpp"
#include "Game/GameObjects/BombFire.hpp"

namespace Bmbz
{
namespace Game
{
    class Bomb : public GameObject
    {
    public:
        void Init(ExplosionTarget&& target, std::function<void()> onDetonation)
        {
            Context().Writer().SetBomb(Id(), target.ownerNumber, target.position);
            Context().Time().AsyncWait(
                _timeLife,
                [this, target = std::move(target), callback = std::move(onDetonation)]() mutable
                {
                    target.map->RemoveBomb(target.position.x, target.position.y);
                    auto fire = Context().Objects().CreateObject<BombFire>();
                    fire->Init(std::move(target));
                    std::move(callback)();
                    Context().Writer().RemoveNetObject(Id());
                    Context().Objects().DeleteObject(this);
                }
            );
        }
    private:
        static constexpr std::chrono::milliseconds _timeLife{2000};
    };
}
}
#endif