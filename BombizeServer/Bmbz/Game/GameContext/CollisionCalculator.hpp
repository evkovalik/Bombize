#ifndef BMBZ_GAME_COLLISIONCALCULATOR_HPP
#define BMBZ_GAME_COLLISIONCALCULATOR_HPP

#include "Game/Components/Colliders/Collider.hpp"

namespace Bmbz
{
namespace Game
{
    class CollisionCalculator
    {
    public:
        void RegisterAsDunamic(Component::Collider* collider)
        {
            _dunamicColliders.push_back(collider);
        }

        void RegisterAsStatic(Component::Collider* collider)
        {
            _staticColliders.push_back(collider);
        }

        void ForgetDunamicCollider(Component::Collider* collider)
        {
            std::erase(_dunamicColliders, collider);
        }

        void ForgetStaticCollider(Component::Collider* collider)
        {
            std::erase(_staticColliders, collider);
        }

        Component::Collider* CheckWithAll(Component::Collider* collider)
        {
            for(auto other : _dunamicColliders)
            {
                if(boost::geometry::intersects(other->Body(), collider->Body()))
                    return other;
            }
            for(auto other : _staticColliders)
            {
                if(boost::geometry::intersects(other->Body(), collider->Body()))
                    return other;
            }
            return nullptr;
        }

        void Calculate()
        {
            for(auto dc : _dunamicColliders)
            {
                for(auto sc : _staticColliders)
                {
                    if(boost::geometry::intersects(dc->Body(), sc->Body()))
                    {
                        dc->IntersectionEvent(*sc);
                        sc->IntersectionEvent(*dc);
                    }
                }
            }
        }

    private:
        std::vector<Component::Collider*> _dunamicColliders;
        std::vector<Component::Collider*> _staticColliders;
    };
}
}
#endif