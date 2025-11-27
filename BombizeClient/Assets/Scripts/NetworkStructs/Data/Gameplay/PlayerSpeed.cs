using System.IO;
using GameNetwork.Types;
using GameNetwork.Interfaces;
using GameNetwork.Utilities;

namespace GameNetwork.NetworkData
{
    public class PlayerSpeed : IDataFromServer
    {
        public int AsInt { get; private set; }
        public float AsFloat { get => _asFloatFixed.Value; }
        private FloatFixed16Bit _asFloatFixed = new(0f, 10f, 0.01f); // set 0, 10, 0.01f

        public void Deserialize(BinaryReader reader)
        {
            AsInt = Utils.ReadByte(reader);
            _asFloatFixed.Deserialize(reader);
        }
    }
}