using System.IO;

namespace GameNetwork.Interfaces
{
    public interface IDataForServer
    {
        void Serialize(BinaryWriter writer);
    }

    public interface IDataFromServer
    {
        void Deserialize(BinaryReader reader);
    }

    public interface INetworkData : IDataForServer, IDataFromServer
    {
    }
}