using System.IO;
using System.Collections.Generic;
using GameNetwork.Types;
using GameNetwork.Utilities;
using UnityEngine;

namespace GameNetwork.NetworkData
{
    public class UserInfo
    {
        public uint Id { get; private set; }
        public NetworkString Nickname { get; private set; }
        public uint RawExperience { get; private set; }
        public uint Level { get; private set; }
        public uint ExperiencePoints { get; private set; }
        public uint Rating { get; private set; }
        public uint CoinsC { get; private set; }
        public uint CoinsQ { get; private set; }
        public CharacterInfo ActiveCharacter { get; private set; }
        public List<CharacterInfo> Characters { get; private set; }

        public UserInfo()
        {
            Nickname = new();
        }

        public void Deserialize(BinaryReader reader)
        {
            Id = Utils.ReadUInt32(reader);
            Nickname.Deserialize(reader);

            RawExperience = Utils.ReadUInt32(reader);
            DefineLevel(RawExperience);

            Rating = Utils.ReadUInt32(reader);
            CoinsC = Utils.ReadUInt32(reader);
            CoinsQ = Utils.ReadUInt32(reader);

            byte activeCharacterIndex = Utils.ReadByte(reader);
            byte countCharacters = Utils.ReadByte(reader);
            Characters = new List<CharacterInfo>(countCharacters);
            for (int i = 0; i < countCharacters; ++i)
            {
                var characterInfo = new CharacterInfo();
                characterInfo.Deserialize(reader);
                Characters.Add(characterInfo);
            }
            ActiveCharacter = Characters[activeCharacterIndex];
        }

        public uint GetMaxExperienceForLevel()
        {
            return (uint)1 << (int)(Level + 8);
        }

        private void DefineLevel(uint rawExperience)
        {
            // [8;31]       bit - define level (bitLevel)
            // [0;bitLevel] bit - define experience points

            // 0b: 00000001 00000000 = 1 lvl and 00000000 points
            //       level| points

            // 0b: 00010011 00100100 = 5 lvl and 0011 00100100 points
            //   level|  points

            for (int bitPos = 31; bitPos >= 8; bitPos--)
            {
                if (((uint)1 << bitPos & rawExperience) != 0)
                {
                    Level = (uint)bitPos - 7;
                    uint mask = ~(uint.MaxValue << bitPos);
                    ExperiencePoints = rawExperience & mask;
                    return;
                }
            }
            // else:
            Level = 0;
            ExperiencePoints = rawExperience & ~(uint.MaxValue << 8);
        }
    }
}