using System;
using System.IO;
using GameNetwork.Utilities;
using UnityEngine;

namespace GameNetwork.NetworkData
{
    public class CharacterInfo
    {
        public byte Id { get; private set; }
        public long DisappearanceTime { get; private set; } = 0;

        public DateTime? GetTime()
        {
            if (DisappearanceTime == 0) return null;
            var epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return epoch.AddTicks(DisappearanceTime);
        }

        public void Deserialize(BinaryReader reader)
        {
            Id = Utils.ReadByte(reader);
            DisappearanceTime = Utils.ReadInt64(reader);
        }
    }
}