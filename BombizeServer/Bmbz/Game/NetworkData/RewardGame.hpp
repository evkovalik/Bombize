#ifndef BMBZ_GAME_REWARD_HPP
#define BMBZ_GAME_REWARD_HPP

#include "Utils/ReadWriteData.hpp"

namespace Bmbz
{
namespace Game
{
namespace NetworkData
{
    struct RewardGame
    {
        RewardGame() = default;

        uint8_t place = 0;
        int32_t experience = 0;
        int32_t rating = 0;
        int32_t coins = 0;

        void Serialize(boost::beast::flat_buffer& buffer) const
        {
            Utils::WriteToBuffer(buffer, place);
            Utils::WriteToBuffer(buffer, experience);
            Utils::WriteToBuffer(buffer, rating);
            Utils::WriteToBuffer(buffer, coins);
        }
    };
}
}
}
#endif