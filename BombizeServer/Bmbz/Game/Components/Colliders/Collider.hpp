#ifndef BMBZ_GAME_COMPONENT_COLLIDER_HPP
#define BMBZ_GAME_COMPONENT_COLLIDER_HPP

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include "NetworkTypes/Vector2.hpp"

namespace Bmbz
{
namespace Game
{
    class GameObject;
    class CollisionCalculator;
    
namespace Component
{
    class Collider
    {
        friend class Game::CollisionCalculator;
    public:
        using Point = boost::geometry::model::d2::point_xy<float>;

        Collider(GameObject& owner)
        : _owner(owner)
        , position(0.0f, 0.0f)
        {}
        virtual ~Collider() = default;

        GameObject& Owner() const { return _owner; }
        NetType::Vector2<float> position;
        virtual const boost::geometry::model::ring<Point>& Body() = 0;

        void SetCollisionListener(std::function<void(Collider&)> listener)
        {
            _listener = listener;
        }
        void ResetCollisionListener() { _listener = nullptr; }
        
        private:
            GameObject& _owner;
            std::function<void(Collider&)> _listener = nullptr;
            void IntersectionEvent(Collider& other)
            {
                if(_listener) _listener(other);
            }
    };
}
}
}
#endif