#ifndef BMBZ_GAME_PLAYERINFO_HPP
#define BMBZ_GAME_PLAYERINFO_HPP

#include "PlayerSpeed.hpp"

namespace Bmbz
{
namespace Game
{
namespace NetworkData
{
    class PlayerInfo
    {
    public:
        // fieldMasks //
        inline static constexpr uint8_t FIELD_ALL = 255;
        inline static constexpr uint8_t FIELD_HEALTH = 1 << 0;
        inline static constexpr uint8_t FIELD_BOMBS = 1 << 1;
        inline static constexpr uint8_t FIELD_SPEED = 1 << 2;
        inline static constexpr uint8_t FIELD_FIRE = 1 << 3;

        bool HasChanges() const noexcept { return _replicateFields != 0; }
        bool HasChanges(uint8_t fieldMask) const noexcept { return _replicateFields & fieldMask; }

        uint8_t Health() const noexcept { return _health; }
        void Health(uint8_t setValue) noexcept
        {
            _health = setValue;
            _replicateFields |= FIELD_HEALTH;
        }

        uint8_t Bombs() const noexcept { return _bombs; }
        void Bombs(uint8_t setValue) noexcept
        {
            _bombs = setValue;
            _replicateFields |= FIELD_BOMBS;
        }

        uint8_t SpeedAsCount() const noexcept { return _speed.asCount; }
        float SpeedAsFloat() const noexcept { return _speed.asFloatFixed.Value(); }

        void Speed(PlayerSpeed setValue)
        {
            _speed = std::move(setValue);
            _replicateFields |= FIELD_SPEED;
        }
        void SpeedAsCount(uint8_t setValue) noexcept
        {
            _speed.asCount = setValue;
            _replicateFields |= FIELD_SPEED;
        }
        void SpeedAsFloat(float setValue)
        {
            if(setValue >= 0) _speed.asFloatFixed.Value(setValue);
            else _speed.asFloatFixed.Value(0.0f);
            _replicateFields |= FIELD_SPEED;
        }

        uint8_t Fire() const noexcept { return _fire; }
        void Fire(uint8_t setValue) noexcept
        {
            _fire = setValue;
            _replicateFields |= FIELD_FIRE;
        }

        void Serialize(boost::beast::flat_buffer& buffer) const
        {
            Utils::WriteToBuffer(buffer, FIELD_ALL);
            Utils::WriteToBuffer(buffer, _health);
            Utils::WriteToBuffer(buffer, _bombs);
            _speed.Serialize(buffer);
            Utils::WriteToBuffer(buffer, _fire);
        }

        void SerializeDiff(boost::beast::flat_buffer& buffer) const
        {
            Utils::WriteToBuffer(buffer, _replicateFields);
            if(_replicateFields & FIELD_HEALTH) Utils::WriteToBuffer(buffer, _health);
            if(_replicateFields & FIELD_BOMBS) Utils::WriteToBuffer(buffer, _bombs);
            if(_replicateFields & FIELD_SPEED) _speed.Serialize(buffer);
            if(_replicateFields & FIELD_FIRE) Utils::WriteToBuffer(buffer, _fire);
        }

        void ClearHistory() { _replicateFields = 0; }

    private:
        uint8_t _replicateFields{0};
        uint8_t _health{0};
        uint8_t _bombs{0};
        PlayerSpeed _speed{};
        uint8_t _fire{0};
    };
}
}
}
#endif