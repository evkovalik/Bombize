using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Types;
using GameNetwork.Utilities;

namespace GameNetwork.NetworkData
{
    public class PlayerPosition : INetworkData
    {
        public Vector2Fixed16Bit Coordinate { get; set; } = new(
            new FloatFixed16Bit(0.0f, 40.0f, 0.01f),
            new FloatFixed16Bit(0.0f, 40.0f, 0.01f)
        );
        public uint Lable { get; set; } = 0;

        public void Serialize(BinaryWriter writer)
        {
            Coordinate.Serialize(writer);
            Utils.WriteAsBinData(writer, Lable);
        }
        public void Deserialize(BinaryReader reader)
        {
            Coordinate.Deserialize(reader);
            Lable = Utils.ReadUInt32(reader);
        }
    }
}