#ifndef BMBZ_GAME_PLAYERSETTINGS_HPP
#define BMBZ_GAME_PLAYERSETTINGS_HPP

#include "Utils/ReadWriteData.hpp"

namespace Bmbz
{
namespace Game
{
namespace NetworkData
{
    struct PlayerSettings
    {
        uint32_t accessKey = 0;
        uint16_t roomNumber = 0;
        uint8_t playerNumber = 0;

        PlayerSettings() = default;

        PlayerSettings(
            uint32_t accessKey,
            uint16_t roomNumber,
            uint8_t playerNumber
        )
        : accessKey(accessKey)
        , roomNumber(roomNumber)
        , playerNumber(playerNumber)
        {}

        void Serialize(boost::beast::flat_buffer& buffer) const
        {
            Utils::WriteToBuffer(buffer, accessKey);
            Utils::WriteToBuffer(buffer, roomNumber);
            Utils::WriteToBuffer(buffer, playerNumber);
        }
    };
}
}
}
#endif