// #ifndef BMBZ_GAME_PLAYERINFO_HPP
// #define BMBZ_GAME_PLAYERINFO_HPP

// #include "NetworkTypes/FloatFixed.hpp"

// namespace Bmbz
// {
// namespace Game
// {
// namespace NetworkData
// {
//     struct PlayerInfo
//     {
//         uint8_t health;
//         NetType::FloatFixed16Bit speed;
//         uint8_t maxCountBomb;
//         uint8_t explosionLength;

//         PlayerInfo(
//             uint8_t health,
//             float speed,
//             uint8_t maxCountBomb,
//             uint8_t explosionLength
//         )
//         : health(health)
//         , speed(NetType::FloatFixed16Bit{0.0f, 5.0f, 0.01f, speed})
//         , maxCountBomb(maxCountBomb)
//         , explosionLength(explosionLength)
//         {}
        
//         void Serialize(boost::beast::flat_buffer& buffer) const
//         {
//             Utils::WriteToBuffer(buffer, health);
//             speed.Serialize(buffer);
//             Utils::WriteToBuffer(buffer, maxCountBomb);
//             Utils::WriteToBuffer(buffer, explosionLength);
//         }
//     };
// }
// }
// }
// #endif