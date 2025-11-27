using UnityEngine;

namespace GameNetwork
{
    public class NetworkUDP : MonoBehaviour
    {
        public UdpDriver UdpDriver { get; private set; }
        [SerializeField] private string IpServer = "127.0.0.1";
        [SerializeField] private int PortServer = 9001;

        private async void Awake()
        {
            UdpDriver = new(IpServer, PortServer);
            await UdpDriver.ReceiveData();
        }

        private void OnDestroy()
        {
            UdpDriver?.Close();
        }
    }
}
