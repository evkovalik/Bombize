#ifndef BMBZ_GAME_GAMEOBJECT_HPP
#define BMBZ_GAME_GAMEOBJECT_HPP

#include <cstdint>
#include <unordered_map>
#include "GameActions/WriterActions.hpp"
#include "Game/GameObjects/GameObjectsTags.hpp"

namespace Bmbz
{
namespace Game
{
    class GameContext;
    class ObjectManager;
    
    class GameObject
    {
        friend class ObjectManager;
    public:
        uint16_t Id() const { return _id; }
        const std::string& Tag() const { return tag; }
    protected:
        GameObject() = default;
        virtual ~GameObject() = default;

        std::string tag = "";
        virtual void Start() {};
        virtual void Update(float deltaTime) {};
        virtual void OnDestroy() {};
        GameContext& Context() { return *_gameContext; }
        
    private:
        GameContext* _gameContext;
        uint16_t _id;
        
        // For ObjectManager //
        void Construct(GameContext* gameContext, uint16_t id)
        {
            _gameContext = gameContext;
            _id = id;
        }
    };
}
}
#endif