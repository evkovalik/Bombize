#ifndef BMBZ_GAME_GAMECONTEXT_HPP
#define BMBZ_GAME_GAMECONTEXT_HPP

#include "CollisionCalculator.hpp"
#include "ObjectManager.hpp"
#include "Time/TimeManager.hpp"
#include "GameActions/WriterActions.hpp"
#include "GameRandom.hpp"

namespace Bmbz
{
namespace Game
{
    class ObjectManager;

    class GameContext
    {
    public:
        GameContext(const boost::asio::strand<boost::asio::any_io_executor>& strand)
        : _timeManager(strand)
        {}

        ObjectManager& Objects() { return _manager; }
        CollisionCalculator& Collisions() { return _collisionCalculator; }
        WriterActions& Writer() { return _writerActions; }
        TimeManager& Time() { return _timeManager; }
        GameRandom& Random() { return _gameRandom; }

    private:
        ObjectManager _manager{*this};
        CollisionCalculator _collisionCalculator;
        WriterActions _writerActions;
        TimeManager _timeManager;
        GameRandom _gameRandom;
    };
}
}
#endif