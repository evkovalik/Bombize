
namespace GameNetwork.Interfaces
{
    public interface ILoginData : INetworkData
    {
        public void FillAsNewUser();
        public void Save();
        public bool Load();
    }
}