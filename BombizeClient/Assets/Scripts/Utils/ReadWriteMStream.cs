using System;
using System.IO;
using GameNetwork.Types;
using GameNetwork.GameActions;
using ServerAPI;

namespace GameNetwork.Utilities
{
    public static partial class Utils
    {
        public static void WriteToStream(MemoryStream toStream, byte value)
        {
            toStream.WriteByte(value);
        }

        public static void WriteToStream(MemoryStream toStream, ushort value)
        {
            var bytes = BitConverter.GetBytes(value);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            toStream.Write(bytes);
        }

        public static void WriteToStream(MemoryStream toStream, uint value)
        {
            var bytes = BitConverter.GetBytes(value);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            toStream.Write(bytes);
        }

        public static void WriteToStream(MemoryStream toStream, ActionType value)
        {
            toStream.WriteByte((byte)value);
        }

        // public static void WriteToStream(MemoryStream toStream, TcpMessageType value)
        // {
        //     WriteToStream(toStream, (ushort)value);
        // }

        public static void WriteToStream(MemoryStream toStream, CltMsgType value)
        {
            WriteToStream(toStream, (ushort)value);
        }

        public static uint ReadUInt32(MemoryStream reader)
        {
            var bytes = new byte[4];
            reader.Read(bytes, 0, bytes.Length);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return BitConverter.ToUInt32(bytes);
        }

        public static byte ReadByte(MemoryStream reader)
        {
            return (byte)reader.ReadByte();
        }
    }
}