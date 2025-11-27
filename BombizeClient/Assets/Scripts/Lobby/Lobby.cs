using UnityEngine;
using GameNetwork.NetworkData;
using ServerAPI;

public class Lobby : MonoBehaviour
{
    [SerializeField] private LobbyView _lobbyView;
    [SerializeField] private CharacterStorageView _characterStorageView;
    [SerializeField] private ShopView _shopView;
    private MainObject _mainObject;

    public void Init(MainObject mainObject)
    {
        _mainObject = mainObject;
        _lobbyView.UpdateUserInfo(_mainObject.Cache.UserInfo);

        _characterStorageView.Init(_mainObject.Cache.UserInfo);
        _shopView.Init(_mainObject.Cache.ShopData);

        _mainObject.Server.OnMsgRunGame += OnRunGame;
    }

    public void ToGameRoom()
    {
        _mainObject.Server.Request(new ReqTryAction(CltMsgType.Play, (Status status)=>
            {
                if(status != Status.Ok) _lobbyView.ResetModeGameWaiting();
            }
        ));
        _lobbyView.SetModeGameWaiting();
    }

    public void CancelToGameRoom()
    {
        _mainObject.Server.Request(new ReqTryAction(CltMsgType.CloseGameRoom, (Status status)=>
            {
                if(status != Status.Ok) Debug.Log("Error: Math already started.");
            }
        ));
        _lobbyView.ResetModeGameWaiting();
    }

    public void BuyCharacter(int id, CurrencyType currencyType)
    {
        _mainObject.Server.Request(new ReqBuyCharacter((byte)id, currencyType, (status, userInfo) =>
        {
            if (status == Status.Ok)
            {
                _lobbyView.UpdateUserInfo(userInfo);
                _characterStorageView.Refresh(userInfo);
            }
            else Debug.Log("Error: ReqBuyCharacter");
        }));
    }

    public void SelectCharacter(int index)
    {
        _mainObject.Server.Request(new ReqSetCharacter((byte)index, (status, userInfo) =>
        {
            if(status == Status.Ok || status == Status.Error)
            {
                _lobbyView.UpdateUserInfo(userInfo);
                _characterStorageView.Hide();
                _characterStorageView.Refresh(userInfo);
            }
        }));
    }


    // <========== PRIVATE ==========> //

    private void OnDestroy()
    {
        _mainObject.Server.OnMsgRunGame -= OnRunGame;
    }

    private void OnRunGame(Status status, GameRoomData data)
    {
        if (status == Status.Ok)
        {
            _mainObject.OpenGameRoom(data);
        }
        else
        {
            // processing...
        }
    }
}