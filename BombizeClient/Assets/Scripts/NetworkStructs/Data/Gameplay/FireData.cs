using System.IO;
using GameNetwork.Interfaces;
using UnityEngine;

public class FireData : IDataFromServer
{
    public byte Left { get; private set; }
    public byte Right { get; private set; }
    public byte Up { get; private set; }
    public byte Down { get; private set; }

    public Vector2 GetScale()
    {
        return new Vector2(Right + Left + 1, Up + Down + 1);    // +1 - center fire
    }

    public Vector2 GetOffset()
    {
        return new Vector2(0.5f * Right - 0.5f * Left, 0.5f * Up - 0.5f * Down);
    }

    public void Deserialize(BinaryReader reader)
    {
        Left = reader.ReadByte();
        Right = reader.ReadByte();
        Up = reader.ReadByte();
        Down = reader.ReadByte();
    }
}