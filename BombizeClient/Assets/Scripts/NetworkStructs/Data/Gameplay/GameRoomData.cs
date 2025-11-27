using System.IO;
using GameNetwork.Interfaces;

namespace GameNetwork.NetworkData
{
    public class GameRoomData : IDataFromServer
    {
        public PlayerSettings PlayerSettings { get; private set; }
        public (int, UserInfo)[] PlayersData { get; private set; }
        public MapInfo MapInfo { get; private set; }

        public void Deserialize(BinaryReader reader)
        {
            PlayerSettings = new();
            PlayerSettings.Deserialize(reader);

            var countPlayers = reader.ReadByte();
            PlayersData = new (int, UserInfo)[countPlayers];
            for (int i = 0; i < countPlayers; i++)
            {
                var playerNumber = reader.ReadByte();
                var ui = new UserInfo();
                ui.Deserialize(reader);
                PlayersData[i] = (playerNumber, ui);
            }
            MapInfo = new();
            MapInfo.Deserialize(reader);
        }
    }
}