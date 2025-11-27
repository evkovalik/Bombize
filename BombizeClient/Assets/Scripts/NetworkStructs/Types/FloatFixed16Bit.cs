using System;
using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Utilities;

namespace GameNetwork.Types
{
    public class FloatFixed16Bit: INetworkData
    {
        private float _value;
        public float Value { get => _value; set => _value = Clamp(value); }
        public float MinValue { get; private set; }
        public float MaxValue { get; private set; }
        public float Precision { get; private set; }

        public FloatFixed16Bit(float minValue, float maxValue, float precision)
        {
            var range = (ulong)((maxValue - minValue) / precision + 1);
            if(range > ushort.MaxValue)
            {
                throw new ArgumentException($"Cannot be placed in 16 bits (Set range = {range} > {ushort.MaxValue})");
            }
            else
            {
                MinValue = minValue;
                MaxValue = maxValue;
                Precision = precision;
                _value = minValue;
            }
        }

        public FloatFixed16Bit(float minValue, float maxValue, float precision, float value)
        : this(minValue, maxValue, precision)
        {
            Value = value;
        }

        public ushort GetAsUInt16() => (ushort)((Value - MinValue) / Precision);

        private float Clamp(float value) => 
        value < MaxValue ? (value > MinValue ? value : MinValue) : MaxValue;

        public void Serialize(BinaryWriter writer)
        {
            Utils.WriteAsBinData(writer, GetAsUInt16());
        }

        public void Deserialize(BinaryReader reader)
        {
            var rawValue = Utils.ReadUInt16(reader);
            Value = (float)rawValue * Precision + MinValue;
        }
    }
}