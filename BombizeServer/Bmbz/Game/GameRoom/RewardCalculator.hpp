#ifndef BMBZ_GAME_REWARDCALCULATOR_HPP
#define BMBZ_GAME_REWARDCALCULATOR_HPP

#include "Participant.hpp"
#include "Game/NetworkData/RewardGame.hpp"
#include "User/UserInfo.hpp"

namespace Bmbz
{
namespace Game
{
    class RewardCalculator
    {
    public:
        void Reserve(size_t size) { _gamersInfo.reserve(size); }

        void AddToCalculations(std::shared_ptr<User::UserInfo> userInfo)
        {
            _gamersInfo.push_back(userInfo);
        }

        void RemoveFromCalculations(std::shared_ptr<User::UserInfo> userInfo)
        {
            std::erase(_gamersInfo, userInfo);
        }

        NetworkData::RewardGame GetReward(std::shared_ptr<User::UserInfo> forWhom, int place)
        {
            if(place > 0)
            {
                NetworkData::RewardGame reward{};
                reward.place = place;
                CalculateReward(reward);
                return std::move(reward);
            }
            else
                return NetworkData::RewardGame{};
        }

    private:
        std::vector<std::shared_ptr<User::UserInfo>> _gamersInfo; // for future

        void CalculateReward(NetworkData::RewardGame& reward)
        {
            if(reward.place == 1)
            {
                reward.experience = 15;
                reward.rating = 20;
                reward.coins = 20;
            }
            else if(reward.place == 2)
            {
                reward.experience = 10;
                reward.rating = 10;
                reward.coins = 10;
            }
            else if(reward.place == 3)
            {
                reward.experience = 5;
                reward.rating = 0;
                reward.coins = 5;
            }
            else
            {
                reward.experience = 5;
                reward.rating = -5;
                reward.coins = 0;
            }
        }
    };
}
}
#endif