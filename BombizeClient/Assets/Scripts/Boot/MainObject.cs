using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using GameNetwork.NetworkData;
using ServerAPI;
using Gameplay;

public class MainObject : MonoBehaviour
{
    public RemoteServer Server { get; private set; }
    public AccountData Cache { get; private set; }
    [SerializeField] private ScreenView _screenView;
    [SerializeField] private List<GameObject> _dontDestroyObjects;
    private Loginer _loginer;

    private void Awake()
    {
        foreach(var obj in _dontDestroyObjects)
        {
            DontDestroyOnLoad(obj);
        }
        Server = new();
        _loginer = new();
    }

    private void Start()
    {
        _screenView.LoadingScreen();
        _loginer.Login(Server, (status, data) =>
        {
            if(status)
            {
                Cache = data;
                OpenLobby();
            }
            else
            {
                // Error login scene...
            }
        });
    }
    
    public void OpenLobby()
    {
        _screenView.LobbyLoadingScreen();
        Action<AsyncOperation> onLoading = null;
        onLoading = (AsyncOperation operation) =>
        {
            operation.completed -= onLoading;
            GameObject.Find("Lobby").GetComponent<Lobby>().Init(this);
            _screenView.FreeScreen();
        };
        var loadOperation = SceneManager.LoadSceneAsync("Scenes/Lobby", LoadSceneMode.Single);
        loadOperation.completed += onLoading;
    }

    public void OpenGameRoom(GameRoomData roomData)
    {
        _screenView.GameRoomLoadingScreen();
        Action<AsyncOperation> onOpenedGameRoom = null;
        onOpenedGameRoom = (AsyncOperation operation) =>
        {
            operation.completed -= onOpenedGameRoom;
            GameObject.Find("GameRoom").GetComponent<GameRoom>().Init(this, roomData);
            _screenView.FreeScreen();
        };
        var loadOperation = SceneManager.LoadSceneAsync("Scenes/GameRoom", LoadSceneMode.Single);
        loadOperation.completed += onOpenedGameRoom;
    }
}
