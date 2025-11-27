using System.IO;

namespace GameNetwork.NetworkData
{
    public class AccountData
    {
        public UserInfo UserInfo { get; set; } = new();
        public ShopData ShopData { get; set; } = new();

        public void Deserialize(BinaryReader reader)
        {
            UserInfo.Deserialize(reader);
            ShopData.Deserialize(reader);
        }
    }
}