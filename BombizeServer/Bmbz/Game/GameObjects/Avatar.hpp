#ifndef BMBZ_GAME_AVATAR_HPP
#define BMBZ_GAME_AVATAR_HPP

#include "Game/GameContext/GameContext.hpp"
#include "Game/GameRoom/InputController/InputControllerObserver.hpp"
#include "Game/Components/Colliders/PlayerCollider.hpp"
#include "Game/NetworkData/PlayerInfo.hpp"

namespace Bmbz
{
namespace Resource {struct CharacterInfo;}
namespace Game
{
    namespace Component { struct IEffect; }
    class Map;

    class Avatar : public GameObject, public InputControllerObserver
    {
    public:
        void Start() override;
        void Update(float deltaTime) override;

        void Init(
            int ownerNumber,
            const Resource::CharacterInfo* characterInfo,
            std::function<void()> onDestroy
        );
        void SetStartPosition(const NetType::Vector2<float>& position, Map* gameMap);
        void ApplyEffect(std::unique_ptr<Component::IEffect>&& effect);
        void Kill(bool informOwner = true);

        int GetOwnerNumber() const noexcept { return _ownerNumber; }
        const Resource::CharacterInfo& GetCharacterInfo() const { return *_characterInfo; }
        const Map& GetGameMap() const { return *_map; }

        void OnInputNewPosition(NetworkData::PlayerPosition position) override;
        void OnInputSetBomb() override;
        
    private:
        int _ownerNumber{-1};
        const Resource::CharacterInfo* _characterInfo = nullptr;
        Map* _map = nullptr;
        std::function<void()> _onDestroyCallback = nullptr;
        NetworkData::PlayerPosition _position;
        Component::PlayerCollider _collider{*this};
        NetworkData::PlayerInfo _playerInfo;
        uint8_t _bombsCreated = 0;
        std::vector<std::unique_ptr<Component::IEffect>> _activeEffects;
        
        inline static constexpr std::chrono::milliseconds DAMAGE_DELAY{1000};
        std::chrono::time_point<std::chrono::steady_clock> _timeLastDamage{};
    };
}
}
#endif