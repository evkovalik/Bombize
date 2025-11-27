using GameNetwork.NetworkData;
using UnityEngine;

namespace DataStructures
{
    public class Player
    {
        public UserInfo UserInfo;
        public PlayerInfo PlayerInfo;
        public GameObject ObjectInScene;

        public Player()
        {
            
        }

        public Player(UserInfo userInfo, PlayerInfo playerInfo, GameObject objectInScene)
        {
            UserInfo = userInfo;
            PlayerInfo = playerInfo;
            ObjectInScene = objectInScene;
        }
    }
}