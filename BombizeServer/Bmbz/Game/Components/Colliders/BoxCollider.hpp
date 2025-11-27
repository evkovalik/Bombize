#ifndef BMBZ_GAME_COMPONENT_BOXCOLLIDER_HPP
#define BMBZ_GAME_COMPONENT_BOXCOLLIDER_HPP

#include "Collider.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
    class BoxCollider : public Collider
    {
    public:
        BoxCollider(GameObject& owner)
        : Collider(owner)
        {}
        
        void Init(
            const NetType::Vector2<float>& position,
            NetType::Vector2<float> scale = {1.0f, 1.0f})
        {
            _scale = std::move(scale);
            _scale.x /= 2.0f; 
            _scale.y /= 2.0f;
            SetPosition(position);
        }

        void SetPosition(const NetType::Vector2<float>& position)
        {
            this->position = position;
            _body = 
            {
                {position.x - _scale.x, position.y - _scale.y},
                {position.x - _scale.x, position.y + _scale.y},
                {position.x + _scale.x, position.y + _scale.y},
                {position.x + _scale.x, position.y - _scale.y},
                {position.x - _scale.x, position.y - _scale.y}
            };
        }

        void SetPosition(const NetType::Vector2<uint8_t>& position)
        {
            SetPosition(NetType::Vector2<float>{
                static_cast<float>(position.x),
                static_cast<float>(position.y)
            });
        }

        const boost::geometry::model::ring<Point>& Body() override
        {
            return _body;
        }
    private:
        //static constexpr float BODY_RADIUS = 0.5f;
        boost::geometry::model::ring<Point> _body;
        NetType::Vector2<float> _scale;
    };
}
}
}
#endif