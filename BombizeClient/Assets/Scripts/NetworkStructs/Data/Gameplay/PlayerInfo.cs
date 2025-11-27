using System;
using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Utilities;

namespace GameNetwork.NetworkData
{
    public class PlayerInfo : IDataFromServer
    {
        public int Health { get; private set; }
        public int Bombs { get; private set; }
        public PlayerSpeed Speed { get; private set; } = new();
        public int Fire { get; private set; }

        public void Deserialize(BinaryReader reader)
        {
            var replicateFields = (Field)Utils.ReadByte(reader);

            if (replicateFields.HasFlag(Field.Health)) Health = Utils.ReadByte(reader);
            if (replicateFields.HasFlag(Field.Bombs)) Bombs = Utils.ReadByte(reader);
            if (replicateFields.HasFlag(Field.Speed)) Speed.Deserialize(reader);
            if (replicateFields.HasFlag(Field.Fire)) Fire = Utils.ReadByte(reader);     
        }

        [Flags]
        private enum Field : byte
        {
            Health = 1 << 0,
            Bombs = 1 << 1,
            Speed = 1 << 2,
            Fire = 1 << 3
        }
    }
}
