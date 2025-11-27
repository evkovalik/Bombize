using System.IO;
using System.Threading;
using WebSocketSharp;

public class WebSocketConnection
{
    private WebSocket _ws;
    private string _URL = "ws://127.0.0.1:9000/";
    private SynchronizationContext _unityContext;
    public delegate void HandlerMessage(MessageEventArgs e);
    public event HandlerMessage OnMessage;

    public WebSocketConnection()
    {
        _unityContext = System.Threading.SynchronizationContext.Current;
        _ws = new WebSocket(_URL);
        _ws.OnMessage += ProcessingMessage;
        Open();
    }

    ~WebSocketConnection()
    {
        _ws.OnMessage -= ProcessingMessage;
        Close();
    }

    public void Open()
    {
        _ws.Connect();
    }

    public void Close()
    {
        _ws.Close();
    }

    public void SendData(byte[] bytesData)
    {
        _ws.Send(bytesData);
    }

    public void SendData(MemoryStream outStream)
    {
        _ws.Send(outStream.ToArray());
        outStream.Dispose();
    }

    public void SendData(MemoryStream outStream, bool close)
    {
        _ws.Send(outStream.ToArray());
        if (close) outStream.Dispose();
    }

    private void ProcessingMessage(object sender, MessageEventArgs e)
    {
        _unityContext.Post(_=>
        {
            OnMessage?.Invoke(e);
        }, null);
    }
}
