using System.IO;
using UnityEngine;

public class TestGameRoom : MonoBehaviour
{
    int i = 0;

    private void Start()
    {
        InvokeRepeating("SendMessage", 1f, 0.5f);
    }

    private void SendMessage()
    {
        Debug.Log($"SendMessage {i++}");
        
        var ms = new MemoryStream();

        using (var writer = new BinaryWriter(ms, System.Text.Encoding.UTF8, true))
        {
            var v = new Vector2Int(10, 5);
            v.SerializeAsByte(writer);
        }

        ms.Position = 0;
        var vi = new Vector2Int();
        using (var reader = new BinaryReader(ms, System.Text.Encoding.UTF8, true))
        {
            vi.DeserializeAsByte(reader);
        }

        Debug.Log($"vi {vi.x}, {vi.y}");
    }

}