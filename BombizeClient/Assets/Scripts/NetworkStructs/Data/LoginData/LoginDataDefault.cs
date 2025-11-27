using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Types;
using UnityEngine;

namespace GameNetwork.NetworkData
{
    public class LoginDataDefault : ILoginData
    {
        public NetworkString Login {  get; private set; }
        public NetworkString Password { get; private set; }
        private const string KeyLogin = "UserLogin";
        private const string KeyPassword = "UserPassword";
        private const string StringForNewUser = "0";

        public void Serialize(BinaryWriter writer)
        {
            if(HasData())
            {
                Login.Serialize(writer);
                Password.Serialize(writer);
            }
        }

        public void Deserialize(BinaryReader reader)
        {
            if(!HasData())
            {
                Login = new();
                Password = new();
            }
            Login.Deserialize(reader);
            Password.Deserialize(reader);
        }

        public void Save()
        {
            if(HasData() && Login.Content != StringForNewUser && Password.Content != StringForNewUser)
            {
                PlayerPrefs.SetString(KeyLogin, Login.Content);
                PlayerPrefs.SetString(KeyPassword, Password.Content);
                PlayerPrefs.Save();
            }
        }

        public bool Load()
        {
            if(PlayerPrefs.HasKey(KeyLogin) && PlayerPrefs.HasKey(KeyPassword))
            {
                Login = new NetworkString(PlayerPrefs.GetString(KeyLogin));
                Password = new NetworkString(PlayerPrefs.GetString(KeyPassword));
                return true;
            }
            return false;
        }

        public void FillAsNewUser()
        {
            Login = new NetworkString(StringForNewUser);
            Password = new NetworkString(StringForNewUser);
        }

        private bool HasData() => Login != null && Password != null;
    }
}
