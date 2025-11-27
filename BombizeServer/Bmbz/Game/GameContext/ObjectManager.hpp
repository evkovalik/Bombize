#ifndef BMBZ_GAME_GAMEOBJECTMANAGER_HPP
#define BMBZ_GAME_GAMEOBJECTMANAGER_HPP

#include <cstdint>
#include <unordered_map>
#include "GameActions/WriterActions.hpp"
#include "GameObject.hpp"

namespace Bmbz
{
namespace Game
{
    class GameContext;

    class ObjectManager
    {
    public:
        ObjectManager(GameContext& context) : _gameContext(context) {}

        template<typename ClassGameObject>
        ClassGameObject* CreateObject()
        {
            static_assert(std::is_base_of_v<GameObject, ClassGameObject>,
                "The class must be inherited from the GameObject");
            
            auto object = new ClassGameObject();
            _gameObjects[_nextObjectId] = object;
            object->Construct(&_gameContext, _nextObjectId);
            object->Start();
            _nextObjectId++;
            return object;
        }

        void DeleteObject(GameObject* object)
        {
            if(_useFrontListForRemove)
                _frontListForRemove.push_back(object);
            else
                _backListForRemove.push_back(object);
        }

        void UpdateAll(float deltaTime)
        {
            for(auto [id, object] : _gameObjects)
            {
                object->Update(deltaTime);
            }
            RemoveGameObjects();
        }

    private:
        uint16_t _nextObjectId = 0;
        std::unordered_map<uint16_t, GameObject*> _gameObjects;
        GameContext& _gameContext;
        
        std::vector<GameObject*> _frontListForRemove;
        std::vector<GameObject*> _backListForRemove;
        bool _useFrontListForRemove = true;

        void RemoveGameObjects()
        {
            if(_useFrontListForRemove && _frontListForRemove.size() > 0)
            {
                _useFrontListForRemove = false;
                for(auto object : _frontListForRemove) RemoveGameObject(object);
                _frontListForRemove.clear();
                RemoveGameObjects();
            }
            else if(!_useFrontListForRemove && _backListForRemove.size() > 0)
            {
                _useFrontListForRemove = true;
                for(auto object : _backListForRemove) RemoveGameObject(object);
                _backListForRemove.clear();
                RemoveGameObjects();
            }
        }

        void RemoveGameObject(GameObject* object)
        {
            object->OnDestroy();
            _gameObjects.erase(object->Id());
            delete object;
            object = nullptr;
        }
    };
}
}
#endif