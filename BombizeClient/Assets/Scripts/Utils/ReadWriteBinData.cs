using System;
using System.IO;
using GameNetwork.GameActions;
using ServerAPI;

namespace GameNetwork.Utilities
{
    public static partial class Utils
    {
        #region Write data
        public static void WriteAsBinData(BinaryWriter writer, ushort value)
        {
            var bytes = BitConverter.GetBytes(value);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            writer.Write(bytes);
        }

        public static void WriteAsBinData(BinaryWriter writer, uint value)
        {
            var bytes = BitConverter.GetBytes(value);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            writer.Write(bytes);
        }

        public static void WriteAsBinData(BinaryWriter writer, int value)
        {
            var bytes = BitConverter.GetBytes(value);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            writer.Write(bytes);
        }

        public static void WriteAsBinData(BinaryWriter writer, ActionType value)
        {
            writer.Write((byte)value);
        }

        #endregion
        #region Read data

        public static byte ReadByte(BinaryReader reader)
        {
            return reader.ReadByte();
        }

        public static ushort ReadUInt16(BinaryReader reader)
        {
            var bytes = reader.ReadBytes(2);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return BitConverter.ToUInt16(bytes);
        }

        public static uint ReadUInt32(BinaryReader reader)
        {
            var bytes = reader.ReadBytes(4);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return BitConverter.ToUInt32(bytes);
        }

        public static int ReadInt32(BinaryReader reader)
        {
            var bytes = reader.ReadBytes(4);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return BitConverter.ToInt32(bytes);
        }

        public static long ReadInt64(BinaryReader reader)
        {
            var bytes = reader.ReadBytes(8);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return BitConverter.ToInt64(bytes);
        }

        public static Status ReadStatus(BinaryReader reader)
        {
            return (Status)reader.ReadByte();
        }

        public static SrvMsgType ReadSrvMsgType(BinaryReader reader)
        {
            var bytes = reader.ReadBytes(2);
            if (!BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return (SrvMsgType)BitConverter.ToUInt16(bytes);
        }

        public static ActionType ReadActionType(BinaryReader reader)
        {
            return (ActionType)reader.ReadByte();;
        }

        #endregion
    }
}