#ifndef BMBZ_GAME_BOMBFIRE_HPP
#define BMBZ_GAME_BOMBFIRE_HPP

#include "Game/GameContext/GameContext.hpp"
#include "Game/Structs/ExplosionTarget.hpp"
#include "Game/Components/Colliders/FireCollider.hpp"
#include "GameResource.hpp"
#include "Avatar.hpp"
#include "Game/Components/Effects/Changes/ChangeHealth.hpp"

namespace Bmbz
{
namespace Game
{
    class BombFire : public GameObject
    {
    public:
        void Init(ExplosionTarget&& target)
        {
            _ownerCharacterInfo = target.characterInfo;
            _fireData = std::move(target.map->ApplyBombFire(target.position.x, target.position.y, target.lengthFire));
            NetType::Vector2<float> colliderPos{
                static_cast<float>(target.position.x), static_cast<float>(target.position.y)
            };
            _collider.Init(colliderPos, _fireData);
            _collider.SetCollisionListener([this](Component::Collider& other){ OnCollision(other); });

            Context().Collisions().RegisterAsStatic(&_collider);
            Context().Writer().SetFire(Id(), target.ownerNumber, target.position, _fireData);
            Context().Time().AsyncWait(
                _timeLife,
                [this, target = std::move(target)]()
                {
                    CreateGameResources();
                    _collider.ResetCollisionListener();
                    Context().Collisions().ForgetStaticCollider(&_collider);
                    Context().Writer().RemoveNetObject(Id());
                    Context().Objects().DeleteObject(this);
                }
            );
        }

    private:
        static constexpr std::chrono::milliseconds _timeLife{200};
        Component::FireCollider _collider{*this};
        NetworkData::FireData _fireData;
        const Resource::CharacterInfo* _ownerCharacterInfo = nullptr;

        void OnCollision(Component::Collider& other)
        {
            if(other.Owner().Tag() == ObjectTag::AVATAR)
            {
                Context().Collisions().ForgetStaticCollider(&_collider); // temp, need delete
                auto& avatar = static_cast<Avatar&>(other.Owner());

                auto damagEffect = std::make_unique<Component::Effect::ChangeHealth>(-1);
                avatar.ApplyEffect(std::move(damagEffect));
                if(_ownerCharacterInfo)
                    avatar.ApplyEffect(std::move(_ownerCharacterInfo->GetAttackEffect()));
            }
        }

        void CreateGameResources()
        {
            auto sides = _fireData.GetDestructions();
            for(auto& destructionPoint : sides)
            {
                if(destructionPoint.has_value())
                {
                    auto resourceType = Context().Random().GetResource();
                    if(resourceType.has_value())
                    {
                        auto resource = Context().Objects().CreateObject<GameResource>();
                        resource->Init(destructionPoint.value(), resourceType.value());
                    }
                }
            }
        }
    };
}
}
#endif