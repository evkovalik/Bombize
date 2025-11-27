#ifndef BMBZ_GAME_BOXCOLLIDER_HPP
#define BMBZ_GAME_BOXCOLLIDER_HPP

#include "GameObject.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/ring.hpp>

namespace Bmbz
{
namespace Game
{
    class BoxCollider : public GameObject
    {
        public:
            using Point = boost::geometry::model::d2::point_xy<float>;

            BoxCollider(
                uint16_t id, 
                NetType::Vector2<uint8_t> position, 
                NetType::Vector2<float> scale = NetType::Vector2<float>{1.0f, 1.0f} // w, h
            )
            : GameObject(id, std::move(position))
            {
                Scale(std::move(scale));
            }

            void Scale(NetType::Vector2<float> scale)
            {
                _scale = scale;
                scale.x /= 2.0f;
                scale.y /= 2.0f;
                _collider.clear();
                boost::geometry::append(_collider, Point(position.x - scale.x, position.y - scale.y));
                boost::geometry::append(_collider, Point(position.x - scale.x, position.y + scale.y));
                boost::geometry::append(_collider, Point(position.x + scale.x, position.y + scale.y));
                boost::geometry::append(_collider, Point(position.x + scale.x, position.y - scale.y));
                boost::geometry::append(_collider, Point(position.x - scale.x, position.y - scale.y));
            }

            const NetType::Vector2<float>& Scale() const
            {
                return _scale;
            }

            const boost::geometry::model::ring<Point>& Collider()
            {
                auto floatPosition = static_cast<NetType::Vector2<float>>(position);
                auto dv = floatPosition - _currentOffsetCollider;
                Point offset(dv.x, dv.y);
                for(auto& point : _collider)
                {
                    boost::geometry::add_point(point, offset);
                }
                _currentOffsetCollider = floatPosition;
                return _collider;
            }

        private:
            NetType::Vector2<float> _scale;
            boost::geometry::model::ring<Point> _collider;
            NetType::Vector2<float> _currentOffsetCollider{0.0f, 0.0f};
    };
}
}
#endif