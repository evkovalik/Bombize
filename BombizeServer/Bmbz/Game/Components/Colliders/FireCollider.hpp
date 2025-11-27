#ifndef BMBZ_GAME_COMPONENT_FIRECOLLIDER_HPP
#define BMBZ_GAME_COMPONENT_FIRECOLLIDER_HPP

#include "Collider.hpp"
#include "Game/NetworkData/FireData.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
    class FireCollider : public Collider
    {
    public:
        FireCollider(GameObject& owner)
        : Collider(owner)
        {}

        void Init(NetType::Vector2<float> position, const NetworkData::FireData& fireData)
        {
            this->position = std::move(position);
            CreateCollider(fireData);
        }
        
        const boost::geometry::model::ring<Point>& Body() override
        {
            return _body;
        }
    
    private:
		boost::geometry::model::ring<Point> _body;
        float _bodyRadius = 0.5f;

        void CreateCollider(const NetworkData::FireData& fireData)
        {
            auto startPoint = Point(position.x - _bodyRadius, position.y + _bodyRadius);
            boost::geometry::append(_body, startPoint);
            
            if(fireData.lengthUp > 0)
            {
                boost::geometry::append(_body, 
                    Point(position.x - _bodyRadius, position.y + _bodyRadius + fireData.lengthUp));
                boost::geometry::append(_body, 
                    Point(position.x + _bodyRadius, position.y + _bodyRadius + fireData.lengthUp));
            }
            boost::geometry::append(
                _body, Point(position.x + _bodyRadius, position.y + _bodyRadius));
            
            if(fireData.lengthRight > 0)
            {
                boost::geometry::append(_body, 
                    Point(position.x + _bodyRadius + fireData.lengthRight, position.y + _bodyRadius));
                boost::geometry::append(_body, 
                    Point(position.x + _bodyRadius + fireData.lengthRight, position.y - _bodyRadius));
            }
            boost::geometry::append(
                _body, Point(position.x + _bodyRadius, position.y - _bodyRadius));
            
            if(fireData.lengthDown > 0)
            {
                boost::geometry::append(_body, 
                    Point(position.x + _bodyRadius, position.y - _bodyRadius - fireData.lengthDown));
                boost::geometry::append(_body, 
                    Point(position.x - _bodyRadius, position.y - _bodyRadius - fireData.lengthDown));
            }
            boost::geometry::append(
                _body, Point(position.x - _bodyRadius, position.y - _bodyRadius));
            
            if(fireData.lengthLeft > 0)
            {
                boost::geometry::append(_body, 
                    Point(position.x - _bodyRadius - fireData.lengthLeft, position.y - _bodyRadius));
                boost::geometry::append(_body, 
                    Point(position.x - _bodyRadius - fireData.lengthLeft, position.y + _bodyRadius));
            }
            boost::geometry::append(_body, std::move(startPoint));
        }
    };
}
}
}
#endif