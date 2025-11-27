using System.IO;
using GameNetwork.Interfaces;

namespace GameNetwork.Types
{
    public struct Vector2Fixed16Bit : INetworkData
    {
        public FloatFixed16Bit x;
        public FloatFixed16Bit y;

        // public Vector2Fixed16Bit(FloatFixed16Bit xy) 
        // {
        //     this.x = xy;
        //     this.y = xy;
        // }

        public Vector2Fixed16Bit(FloatFixed16Bit x, FloatFixed16Bit y) 
        {
            this.x = x;
            this.y = y;
        }

        public void Value(float setX, float setY)
        {
            x.Value = setX;
            y.Value = setY;
        }

        public void Deserialize(BinaryReader reader)
        {
            x.Deserialize(reader);
            y.Deserialize(reader);
        }

        public void Serialize(BinaryWriter writer)
        {
            x.Serialize(writer);
            y.Serialize(writer);
        }
    }
}