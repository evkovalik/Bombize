using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

public class UdpDriver
{
    public Action<byte[]> OnReceive;

    private UdpClient _udpClient;
    private IPEndPoint _remoteEndPoint;

    public UdpDriver(string ip, int port)
    {
        _udpClient = new UdpClient();
        _remoteEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
    }

    public async Task ReceiveData()
    {
        try
        {
            while (true)
            {
                UdpReceiveResult result = await _udpClient.ReceiveAsync();
                OnReceive?.Invoke(result.Buffer);
            }
        }
        catch (Exception)
        {
            //Debug.LogError(e);
        }
    }

    //public async Task SendData(byte[] bytes) 
    public async void SendData(byte[] bytes)
    {
        await _udpClient.SendAsync(bytes, bytes.Length, _remoteEndPoint);
    }

    public void SendData(MemoryStream outStream)
    {
        SendData(outStream.ToArray());
        outStream.Dispose();
    }

    public void SendData(MemoryStream outStream, bool close)
    {
        SendData(outStream.ToArray());
        if (close) outStream.Dispose();
    }

    public void Close()
    {
        _udpClient.Close();
    }
}
