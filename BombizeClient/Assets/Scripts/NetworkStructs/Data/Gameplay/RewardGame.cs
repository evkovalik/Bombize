using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Utilities;

namespace GameNetwork.NetworkData
{
    public class RewardGame : IDataFromServer
    {
        public int Place {get; private set;}
        public int Experience {get; private set;}
        public int Rating { get; private set; }
        public int Coins {get; private set;}

        public RewardGame(){}

        public void Deserialize(BinaryReader reader)
        {
            Place = Utils.ReadByte(reader);
            Experience = Utils.ReadInt32(reader);
            Rating = Utils.ReadInt32(reader);
            Coins = Utils.ReadInt32(reader);
        }
    }
}

