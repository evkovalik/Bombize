using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Utilities;

namespace GameNetwork.NetworkData
{
    public class PlayerSettings : IDataFromServer
    {
        public uint AccessKey { get; private set; }
        public ushort RoomNumber { get; private set; }
        public byte PlayerNumber { get; private set; }

        public void Deserialize(BinaryReader reader)
        {
            AccessKey = Utils.ReadUInt32(reader);
            RoomNumber = Utils.ReadUInt16(reader);
            PlayerNumber = reader.ReadByte();
        }
    }
}

