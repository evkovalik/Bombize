using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Utilities;

public enum CurrencyType : byte
{
    Coin1,
    Coin2
}

public struct Currency : INetworkData
{
    public CurrencyType type;
    public uint value;

    public Currency(CurrencyType type, uint value)
    {
        this.type = type;
        this.value = value;
    }

    public void Deserialize(BinaryReader reader)
    {
        type = (CurrencyType)Utils.ReadByte(reader);
        value = Utils.ReadUInt32(reader);
    }

    public void Serialize(BinaryWriter writer)
    {
        Utils.WriteAsBinData(writer, (byte)type);
        Utils.WriteAsBinData(writer, value);
    }
}