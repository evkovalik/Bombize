using System.IO;
using UnityEngine;

public class TestView : MonoBehaviour
{
    public PlayerLifeView _lifeView;

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Q))
        {
            _lifeView.Value = 1;
        }
        else if (Input.GetKeyDown(KeyCode.W))
        {
            _lifeView.Value = 2;
        }
        else if (Input.GetKeyDown(KeyCode.E))
        {
            _lifeView.Value = 3;
        }
        else if (Input.GetKeyDown(KeyCode.X))
        {
            _lifeView.Value = 0;
        }
    }
}