using System;
using ServerAPI;

namespace GameNetwork.Utilities
{
    public static partial class Utils
    {
        public static T ConvertToFixed<T>(float number, float minValue, float precision)
        {
            return (T)Convert.ChangeType((number - minValue) / precision, typeof(T));
        }

        public static float ConvertFromFixed<T>(T number, float minValue, float precision)
        {
            return (float)Convert.ChangeType(number, typeof(float)) * precision + minValue;
        }

        public static byte[] ConvertToBytes(CltMsgType value)
        {
            var bytes = BitConverter.GetBytes((ushort)value);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return bytes;
        }
    }
}