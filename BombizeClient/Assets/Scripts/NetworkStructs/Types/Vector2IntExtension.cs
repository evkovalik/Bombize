using System.IO;
using UnityEngine;

public static class Vector2IntExtension
{
    public static void DeserializeAsByte(this ref Vector2Int vector, BinaryReader reader)
    {
        vector.x = reader.ReadByte();
        vector.y = reader.ReadByte();
    }

    public static void SerializeAsByte(this ref Vector2Int vector, BinaryWriter writer)
    {
        writer.Write((byte)vector.x);
        writer.Write((byte)vector.y);
    }
}