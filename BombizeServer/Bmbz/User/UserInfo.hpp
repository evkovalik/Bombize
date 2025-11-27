#ifndef BMBZ_USERINFO_HPP
#define BMBZ_USERINFO_HPP

#include <cstdint>
#include <vector>
#include "Character.hpp"
#include "NetworkTypes/NetworkString.hpp"
#include "NetworkTypes/WrapNetworkArray256.hpp"

namespace Bmbz
{
namespace User
{
    struct UserInfo
    {
        UserInfo() = default;

        uint32_t id;
        NetType::NetworkString nickname;
        uint32_t experience;
        uint32_t rating;
        uint32_t coinsC;
        uint32_t coinsQ;
        std::vector<Character> characters;
        uint8_t activeCharacterIndex;

        void UpdateCharactersList()
        {
            for(auto it = characters.begin(); it != characters.end(); )
            {
                if (it->disappearanceTime.has_value()
                && std::chrono::system_clock::now() >= it->disappearanceTime.value())
                {
                    it = characters.erase(it);
                    uint8_t index = static_cast<uint8_t>(std::distance(characters.begin(), it));
                    if(index == activeCharacterIndex || index < activeCharacterIndex)
                    {
                        activeCharacterIndex = 0;
                    }
                }
                else
                    ++it;
            }
        }

        void SetCharacterById(uint8_t id)
        {
            for(int index = 0; index < characters.size(); index++)
                if(characters[index].id == id)
                {
                    activeCharacterIndex = index;
                    break;
                }
        }

        void Serialize(boost::beast::flat_buffer& buffer)
        {
            Utils::WriteToBuffer(buffer, id);
            nickname.Serialize(buffer);
            Utils::WriteToBuffer(buffer, experience);
            Utils::WriteToBuffer(buffer, rating);
            Utils::WriteToBuffer(buffer, coinsC);
            Utils::WriteToBuffer(buffer, coinsQ);
            Utils::WriteToBuffer(buffer, activeCharacterIndex);

            UpdateCharactersList();
            Utils::WriteToBuffer(buffer, static_cast<uint8_t>(characters.size()));
            for(const auto& character : characters)
            {
                Utils::WriteToBuffer(buffer, static_cast<uint8_t>(character.id));
                Utils::WriteToBuffer(buffer, character.GetDisappearanceTimeAsInt64());
                std::cout << "character.id = " << character.id << ", time = " << character.GetDisappearanceTimeAsInt64() << "\n";
            }
        }
    };
}
}

#endif