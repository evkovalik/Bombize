using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Utilities;

namespace GameNetwork.Types
{
    public struct FloatAsUInt : INetworkData
    {
        public float value;

        public FloatAsUInt(float value)
        {
            this.value = value;
        }

        public uint GetAsUInt()
        {
            return (uint)(value * 1000);
        }

        public void Deserialize(BinaryReader reader)
        {
            value = (float)Utils.ReadUInt32(reader);
        }

        public void Serialize(BinaryWriter writer)
        {
            Utils.WriteAsBinData(writer, GetAsUInt());
        }
    }
}