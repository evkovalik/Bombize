#ifndef BMBZ_GAME_GROUPTOGAME_HPP
#define BMBZ_GAME_GROUPTOGAME_HPP

#include <array>

namespace Bmbz
{
    class UserConnection;
namespace Game
{
    struct GroupToGame
    {
        static constexpr int CAPACITY = 4;
        GroupToGame() = default;

        int Count() const noexcept { return _count; }
        int Capacity() const noexcept { return CAPACITY; }

        void AddGamer(std::shared_ptr<UserConnection> gamer) noexcept
        {
            if(_count < CAPACITY) _gamers[_count++] = gamer;
        }

        auto ExtractNextGamer() noexcept
        {
            return (_count > 0) ? _gamers[--_count] : nullptr;
        }

    private:
        std::array<std::shared_ptr<UserConnection>, CAPACITY> _gamers;
        int _count = 0;
    };
}
}
#endif