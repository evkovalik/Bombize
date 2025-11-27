#ifndef BMBZ_SHOP_HPP
#define BMBZ_SHOP_HPP

#include "User/UserInfo.hpp"
#include "Resources/ResourceLocator.hpp"

namespace Bmbz
{
    class Shop
    {
    public:
        enum class CoinType : uint8_t
        {
            Coin1,
            Coin2
        };

        bool BuyCharacter(std::shared_ptr<User::UserInfo>userInfo, uint8_t idForBuy, CoinType coinType)
        {
            if(idForBuy <= 0 || idForBuy >= ResourceLocator::GetCharacters().Count())
                return false;

            auto& item = ResourceLocator::GetCharacters().GetCharacterInfo(idForBuy);

            if(coinType == CoinType::Coin1 && userInfo->coinsC >= item.price1
            || coinType == CoinType::Coin2 && userInfo->coinsQ >= item.price2)
            {
                if(coinType == CoinType::Coin1)
                    userInfo->coinsC -= item.price1;
                else
                    userInfo->coinsQ -= item.price2;

                auto characterIt = std::find_if(userInfo->characters.begin(), userInfo->characters.end(),
                    [&idForBuy](User::Character& character){ return character.id == idForBuy; }
                );

                if(characterIt == userInfo->characters.end())
                    userInfo->characters.emplace_back(
                        User::Character{idForBuy, std::chrono::system_clock::now() + RENTAL_TIME}
                    );
                else
                    characterIt->disappearanceTime.value() += RENTAL_TIME;

                userInfo->SetCharacterById(idForBuy);
                return true;
            }
            else return false;
        }

        void SerializeItemsForSale(boost::beast::flat_buffer& buffer) const
        {
            uint8_t startIndex = 1;
            uint8_t stopIndex = static_cast<uint8_t>(ResourceLocator::GetCharacters().Count());
            uint8_t count = stopIndex - startIndex;
            Utils::WriteToBuffer(buffer, count);
            for(startIndex; startIndex < stopIndex; startIndex++)
            {
                auto& item = ResourceLocator::GetCharacters().GetCharacterInfo(startIndex);
                Utils::WriteToBuffer(buffer, startIndex);
                Utils::WriteToBuffer(buffer, item.price1);
                Utils::WriteToBuffer(buffer, item.price2);
            }
        }
        
    private:
        inline static constexpr std::chrono::hours RENTAL_TIME{24};
    };
}
#endif