using System.Collections.Generic;
using UnityEngine;
using DataStructures;
using GameNetwork.NetworkData;
using GameNetwork.GameActions;
using GameNetwork;
using ServerAPI;

namespace Gameplay
{
    public class GameRoom : MonoBehaviour
    {
        [SerializeField] private GameRoomView _roomView;
        [SerializeField] private NetworkUDP _networkUDP;
        [SerializeField] private Map _map;
        private MainObject _mainObject;
        private ActionDispatcher _dispatcher;
        private GameRoomData _roomData;
        private int _selfNumber;
        private Dictionary<int, Player> _players;
        private NetPlayerController _playerController;
        private Dictionary<int, GameObject> _networkObjects = new(); // netObjectId - object
        private readonly Vector3 _offsetResources = new(0.5f, 0.5f, 0);
        private readonly Vector3 _scalePlayerObject = new(0.9f, 0.9f, 1f);

        public void Init(MainObject mainObject, GameRoomData roomData)
        {
            _mainObject = mainObject;
            _roomData = roomData;

            _dispatcher = new(
                _networkUDP.UdpDriver,
                roomData.PlayerSettings.AccessKey,
                roomData.PlayerSettings.RoomNumber
            );
            _selfNumber = roomData.PlayerSettings.PlayerNumber;

            _mainObject.Server.OnMsgRewardGame += GiveReward;

            BindActions();
            _map.Init(roomData.MapInfo);
            CreatePlayers(roomData.PlayersData);

            InvokeRepeating(nameof(MaintainingNetworkActivity), 0, .1f);
        }

        private void MaintainingNetworkActivity()
        {
            _dispatcher.ConfirmActivity();
        }

        private void CreatePlayers((int, UserInfo)[] playersData)
        {
            _players = new(playersData.Length);
            foreach (var (number, info) in playersData)
            {
                _players.Add(number, new Player());
                _players[number].UserInfo = info;
            }
        }

        private void CreateAvatar(int ownerNumber, PlayerInfo playerInfo, Vector2Int spawnPosition)
        {
            var ui = _players[ownerNumber].UserInfo;
            var obj = Instantiate(
                    Locator.GameStorage.Instance.Characters[ui.ActiveCharacter.Id].Prefab,
                    new(spawnPosition.x, spawnPosition.y),
                    Quaternion.identity
            );
            obj.transform.localScale = _scalePlayerObject;

            if (ownerNumber == _selfNumber)
            {
                CancelInvoke(nameof(MaintainingNetworkActivity));

                _playerController = obj.AddComponent<NetPlayerController>();
                _playerController.Init(_dispatcher, playerInfo.Speed.AsFloat);
                _roomView.UpdatePlayerInfo(playerInfo);
            }
            else
            {
                obj.AddComponent<NetOpponentController>();
            }
            _players[ownerNumber].PlayerInfo = playerInfo;
            _players[ownerNumber].ObjectInScene = obj;
        }

        private void FinishGame()
        {
            foreach (var (number, player) in _players)
            {
                if (player.ObjectInScene != null) Destroy(player.ObjectInScene);
            }
            foreach (var (id, netGameObject) in _networkObjects)
            {
                Destroy(netGameObject);
            }
            _players.Clear();
            _networkObjects.Clear();
            CancelInvoke(nameof(MaintainingNetworkActivity));
            _mainObject.Server.OnMsgRewardGame -= GiveReward;
        }

        // <========== TCP ==========> //
        #region tcp methods
        private void GiveReward(Status status, RewardGame reward)
        {
            FinishGame();
            if (status == Status.Ok) _roomView.ShowReward(reward);
            else OpenLobby();
        }

        public void Close()
        {
            _mainObject.Server.Request(new ReqLeaveGame(
                (status, reward) =>
                {
                    if (status != Status.Ok) Debug.Log("Error status: ReqLeaveGame");
                    else
                    {
                        FinishGame();
                        _roomView.ShowReward(reward);
                    }
                }
            ));
        }

        public void OpenLobby()
        {
            _mainObject.Server.Request(new ReqGetUserInfo(
                (status, userInfo) =>
                {
                    if (status == Status.Ok)
                    {
                        _mainObject.Cache.UserInfo = userInfo;
                        _mainObject.OpenLobby();
                    }
                    else
                        Debug.Log("Error status: ReqGetUserInfo");
                }
            ));
        }

        #endregion

        // <========== UDP ==========> //
        #region Binding UDP

        private void BindActions()
        {
            _dispatcher.ActCreateAvatar += CreateAvatar;
            _dispatcher.ActUpdatePosition += UpdateAvatarPosition;
            _dispatcher.ActSetBomb += CreateBomb;
            _dispatcher.ActSetFire += CreateFire;
            _dispatcher.ActRemoveObject += RemoveObject;
            _dispatcher.ActUpdatePlayerInfo += UpdatePlayerInfo;
            _dispatcher.ActReplaceMapCell += ReplaceMapCell;
            _dispatcher.ActCreateResource += CreateResource;
        }

        private void UnbindAllActions()
        {

        }

        #endregion

        // <========== Game action handlers ==========> //

        public void UpdateAvatarPosition(int number, PlayerPosition nextPosition)
        {
            if (number != _selfNumber)
            {
                _players[number].ObjectInScene.GetComponent<NetOpponentController>()
                .NextPlayerPosition = nextPosition;
            }
        }

        public void CreateBomb(int netId, int owner, Vector2Int position)
        {
            var characterId = _players[owner].UserInfo.ActiveCharacter.Id;
            var obj = Instantiate(
                Locator.GameStorage.Instance.Characters[characterId].BombPrefab,
                new(position.x, position.y),
                Quaternion.identity
            );
            _networkObjects.Add(netId, obj);
        }

        public void CreateFire(int netId, int owner, Vector2Int position, FireData data)
        {
            var characterId = _players[owner].UserInfo.ActiveCharacter.Id;
            var obj = Instantiate(
                Locator.GameStorage.Instance.Characters[characterId].FirePrefab,
                new(position.x, position.y),
                Quaternion.identity
            );
            obj.GetComponent<Fire>().SetSizeAndOffset(data.GetScale(), data.GetOffset());
            _networkObjects.Add(netId, obj);
        }

        public void RemoveObject(int netId)
        {
            if (_networkObjects.ContainsKey(netId))
            {
                Destroy(_networkObjects[netId]);
                _networkObjects.Remove(netId);
            }
        }

        public void UpdatePlayerInfo(int number, PlayerInfo playerInfo)
        {
            if (number == _selfNumber)
            {
                _roomView.UpdatePlayerInfo(playerInfo);
                _playerController.Speed = playerInfo.Speed.AsFloat;
            }
        }

        public void ReplaceMapCell(Vector2Int position, int newCellValue)
        {
            _map.ReplaceCell(position, newCellValue);
        }

        public void CreateResource(int netId, Vector2Int position, int resourceIndex)
        {
            var obj = Instantiate(
                Locator.GameStorage.Instance.MapResources[resourceIndex],
                _map.ToWorldPosition(position) + _offsetResources,
                Quaternion.identity
            );
            _networkObjects.Add(netId, obj);
        }

        public void KillPlayer(int playerNumber)
        {
            _players[playerNumber].ObjectInScene.GetComponent<CircleCollider2D>().enabled = false;
            if (_selfNumber == playerNumber)
            {
                Destroy(_playerController);
                _playerController = null;
            }
            else
            {
                Destroy(_players[playerNumber].ObjectInScene.GetComponent<NetOpponentController>());
            }
            // remove game object with delay...
            Destroy(_players[playerNumber].ObjectInScene); // temp
        }

    }
}