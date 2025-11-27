#ifndef BMBZ_GAME_GAMERESOURCE_HPP
#define BMBZ_GAME_GAMERESOURCE_HPP

#include "Game/GameContext/GameContext.hpp"
#include "Game/Components/Colliders/BoxCollider.hpp"

#include "Game/Structs/GameResourceType.hpp"
#include "Game/Components/Effects/Changes/ChangeBomb.hpp"
#include "Game/Components/Effects/Changes/ChangeSpeed.hpp"
#include "Game/Components/Effects/Changes/ChangeFire.hpp"

namespace Bmbz
{
namespace Game
{
    class GameResource : public GameObject
    {
    public:
        void Init(const NetType::Vector2<uint8_t>& position, GameResourceType resourceType)
        {
            if(resourceType == GameResourceType::AddBomb)
                _effect = std::make_unique<Component::Effect::ChangeBomb>(CHANGES_COUNT);
            else if(resourceType == GameResourceType::AddSpeed)
                _effect = std::make_unique<Component::Effect::ChangeSpeed>(CHANGES_COUNT);
            else if(resourceType == GameResourceType::AddFire)
                _effect = std::make_unique<Component::Effect::ChangeFire>(CHANGES_COUNT);
            else
            {
                Context().Objects().DeleteObject(this);
                return;
            }

            _collider.Init(NetType::Vector2<float>{
                static_cast<float>(position.x),
                static_cast<float>(position.y)
            });
            Context().Collisions().RegisterAsStatic(&_collider);
            _collider.SetCollisionListener([this](Component::Collider& other){ OnCollision(other); });
            Context().Writer().CreateResource(Id(), position, resourceType);
        }
    private:
        static constexpr int CHANGES_COUNT = 1;
        Component::BoxCollider _collider{*this};
        std::unique_ptr<Component::IEffect> _effect;
        
        void OnCollision(Component::Collider& other)
        {
            if(other.Owner().Tag() == ObjectTag::AVATAR)
            {
                auto& avatar = static_cast<Avatar&>(other.Owner());
                avatar.ApplyEffect(std::move(_effect));

                Context().Collisions().ForgetStaticCollider(&_collider);
                _collider.ResetCollisionListener();
                Context().Writer().RemoveNetObject(Id());
                Context().Objects().DeleteObject(this);
            }
        }
    };
}
}
#endif