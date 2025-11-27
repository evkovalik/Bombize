using System.IO;
using System.Text;
using System;
using GameNetwork.Interfaces;

namespace GameNetwork.Types
{
    public class NetworkString : INetworkData
    {
        public const int MaxLength = 60;
        private string _content;
        public string Content
        {
            get => _content;
            set
            {
                if(IsValid(value))
                {
                    _content = value;
                }
                else
                {
                    throw new ArgumentOutOfRangeException(nameof(value),
                        "The maximum number of characters should not exceed " + MaxLength);
                }
            }
        }

        public NetworkString()
        {
            _content = "";
        }

        public NetworkString(string str)
        {
            Content = str;
        }

        private bool IsValid(string str)
        {
            return str.Length <= MaxLength;
        }

        public void Serialize(BinaryWriter writer)
        {
            byte[] bytesText = Encoding.UTF8.GetBytes(Content);
            writer.Write((byte)bytesText.Length);
            writer.Write(bytesText);
        }

        public void Deserialize(BinaryReader reader)
        {
            byte len = reader.ReadByte();
            byte[] bytesText = reader.ReadBytes(len);
            Content = Encoding.UTF8.GetString(bytesText);
        }
    }
}