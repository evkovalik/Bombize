#ifndef BMBZ_GAME_COMPONENT_PLAYERCOLLIDER_HPP
#define BMBZ_GAME_COMPONENT_PLAYERCOLLIDER_HPP

#include "Collider.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
    class PlayerCollider : public Collider
    {
    public:
        PlayerCollider(GameObject& owner)
        : Collider(owner)
        {
            SetBodyInCenterPosition();
        }

        const boost::geometry::model::ring<Point>& Body() override
        {
            auto dv = position - _currentOffsetBody;
            Point offset(dv.x, dv.y);
            for(auto& point : _body)
            {
                boost::geometry::add_point(point, offset);
            }
            _currentOffsetBody = position;
            return _body;
        }
    
    private:
        NetType::Vector2<float> _currentOffsetBody{0.0f, 0.0f};

		boost::geometry::model::ring<Point> _body
		{
			{0.0f, 0.3f}, {0.0f, 0.6f},
			{0.3f, 0.9f}, {0.6f, 0.9f},
			{0.9f, 0.6f}, {0.9f, 0.3f},
			{0.6f, 0.0f}, {0.3f, 0.0f},
			{0.0f, 0.3f}
		};
        float _bodyRadius = 0.45f;

        void SetBodyInCenterPosition()
        {
            Point offset{_bodyRadius, _bodyRadius};
            for(auto& point : _body)
                boost::geometry::subtract_point(point, offset);
        }
    };
}
}
}
#endif