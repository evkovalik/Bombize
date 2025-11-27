#include "Avatar.hpp"
#include "Game/GameObjects/Map.hpp"
#include "Game/GameObjects/Bomb.hpp"
#include "Game/Structs/ExplosionTarget.hpp"
#include "Utils/DataConversion.hpp"
#include "Game/Components/Effects/IEffect.hpp"
#include "Resources/Characters/CharacterInfo.hpp"

namespace Bmbz
{
namespace Game
{
    void Avatar::Start()
    {
        tag = ObjectTag::AVATAR;
    }

    void Avatar::Init(
        int ownerNumber,
        const Resource::CharacterInfo* characterInfo,
        std::function<void()> onDestroy)
    {
        _ownerNumber = ownerNumber;
        _characterInfo = characterInfo;
        _onDestroyCallback = onDestroy;

        _playerInfo.Health(3);
        _playerInfo.Bombs(1);
        _playerInfo.Speed(NetworkData::PlayerSpeed{1});
        _playerInfo.Fire(1);

        Context().Collisions().RegisterAsDunamic(&_collider);
        Context().Writer().CreateAvatar(ownerNumber, _playerInfo, _position.coordinate);
    }

    void Avatar::SetStartPosition(const NetType::Vector2<float>& position, Map* gameMap)
    {
        _position.coordinate.Value(position.x, position.y);
        _collider.position.Value(_position.coordinate.x.Value(), _position.coordinate.y.Value());
        _map = gameMap;
    }

    void Avatar::ApplyEffect(std::unique_ptr<Component::IEffect>&& effect)
    {
        if(_playerInfo.Health() == 0) return;

        auto health = _playerInfo.Health();
        effect->Execute(_playerInfo);

        if(_playerInfo.Health() == 0) // fatal damage
        {
            Kill();
        }
        else
        {
            if(_playerInfo.Health() < health) // damage
            {
                auto now = std::chrono::steady_clock::now();
                if(now >= _timeLastDamage + DAMAGE_DELAY)
                {
                    auto selfEffect = _characterInfo->GetProtectionEffect();
                    selfEffect->Execute(_playerInfo);
                    if(selfEffect->IsActive())
                        _activeEffects.push_back(std::move(effect));
    
                    _timeLastDamage = now;
                }
                else
                {
                    _playerInfo.Health(health); // restore
                }
            }
            else
            {
                if(effect->IsActive())
                    _activeEffects.push_back(std::move(effect));
            }
        }
    }

    void Avatar::Kill(bool informOwner)
    {
        _playerInfo.Health(0);
        Context().Writer().UpdatePlayerInfo(_ownerNumber, _playerInfo);
        Context().Collisions().ForgetDunamicCollider(&_collider);
        Context().Objects().DeleteObject(this);

        if(informOwner) _onDestroyCallback();
    }
    
    void Avatar::Update(float deltaTime)
    {
        if(_activeEffects.size() > 0)
        {
            for(auto effectIt = _activeEffects.begin(); effectIt != _activeEffects.end(); )
            {
                (*effectIt)->Execute(_playerInfo);
                if(!(*effectIt)->IsActive())
                    effectIt = _activeEffects.erase(effectIt);
                else
                    ++effectIt;
            }
        }

        Context().Writer().UpdateAvatarPosition(_ownerNumber, _position);
        if(_playerInfo.HasChanges())
        {
            Context().Writer().UpdatePlayerInfo(_ownerNumber, _playerInfo);
            _playerInfo.ClearHistory();
        }
    }

    void Avatar::OnInputNewPosition(NetworkData::PlayerPosition position)
    {
        if(position.lable > _position.lable)
        {
            // control...
            _position = std::move(position);
            _collider.position.Value(_position.coordinate.x.Value(), _position.coordinate.y.Value());
        }
    }

    void Avatar::OnInputSetBomb()
    {
        if(_bombsCreated >= _playerInfo.Bombs()) return;
        
        NetType::Vector2<uint8_t> bombPosition{
            Utils::RoundTo<uint8_t>(_position.coordinate.x.Value()),
            Utils::RoundTo<uint8_t>(_position.coordinate.y.Value())
        };
        bool success = _map->TrySetBomb(bombPosition.x, bombPosition.y);
        if(!success) return;

        auto bomb = Context().Objects().CreateObject<Bomb>();
        ExplosionTarget target{
            .characterInfo = _characterInfo,
            .map = _map,
            .position = std::move(bombPosition),
            .lengthFire = _playerInfo.Fire(),
            .ownerNumber = static_cast<uint8_t>(_ownerNumber)
        };
        bomb->Init(std::move(target), [this](){ _bombsCreated--; }); // now not save!!!
        _bombsCreated++;
    }
}
}