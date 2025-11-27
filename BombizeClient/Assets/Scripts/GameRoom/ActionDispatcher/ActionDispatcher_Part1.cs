using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using GameNetwork.NetworkData;
using GameNetwork.Utilities;
using UnityEngine;

namespace GameNetwork.GameActions
{
    public partial class ActionDispatcher
    {
        // ----- Actins from server to client ----- //

        private UdpPacketDeliverer _deliverer;
        private Dictionary<ActionType, Action<BinaryReader>> _actionsStorage;

        public ActionDispatcher(UdpDriver diver, uint accessKey, uint roomNumber)
        {
            _deliverer = new(diver, accessKey, (ushort)roomNumber);
            RegisterGameActions();
            _deliverer.OnInputGameAction += ReadAndProcessActions;
        }

        ~ActionDispatcher()
        {
            _deliverer.OnInputGameAction -= ReadAndProcessActions;
        }

        public event Action<int, PlayerInfo, Vector2Int> ActCreateAvatar;
        public event Action<int, PlayerPosition> ActUpdatePosition;
        public event Action<int, int, Vector2Int> ActSetBomb;
        public event Action<int, int, Vector2Int, FireData> ActSetFire;
        public event Action<int> ActRemoveObject;
        public event Action<int, PlayerInfo> ActUpdatePlayerInfo;
        public event Action<Vector2Int, int> ActReplaceMapCell;
        public event Action<int, Vector2Int, int> ActCreateResource;

        // public event Action<int> GAPlayerDeath;

        private void ReadAndProcessActions(MemoryStream inMStream)
        {
            try
            {
                using var reader = new BinaryReader(inMStream, Encoding.UTF8, true);
                while (inMStream.Position < inMStream.Length)
                {
                    var inActionType = Utils.ReadActionType(reader);
                    //Debug.Log($"Input game action: {inActionType}");
                    _actionsStorage[inActionType].Invoke(reader);
                }
            }
            catch (Exception error)
            {
                Debug.LogError(error);
            }
        }

        private void RegisterGameActions()
        {
            _actionsStorage = new()
            {
                { ActionType.CreateAvatar, CreateAvatar },
                { ActionType.UpdatePosition, UpdatePlayerPosition },
                { ActionType.SetBomb, SetBomb },
                { ActionType.SetFire, SetFire },
                { ActionType.RemoveObject, RemoveObject },
                { ActionType.UpdatePlayerInfo, UpdatePlayerInfo },
                { ActionType.ReplaceCell, ReplaceCell },
                { ActionType.CreateResource, CreateResource }

                // { ActionType.PlayerDeath, PlayerDeath }
            };
        }

        // ----- implementation ----- //

        private void CreateAvatar(BinaryReader reader)
        {
            var ownerNumber = Utils.ReadByte(reader);
            PlayerInfo playerInfo = new();
            playerInfo.Deserialize(reader);
            Vector2Int spawnPos = new();
            spawnPos.DeserializeAsByte(reader);
            ActCreateAvatar?.Invoke(ownerNumber, playerInfo, spawnPos);
        }

        private void UpdatePlayerPosition(BinaryReader reader)
        {
            var playerNumber = Utils.ReadByte(reader);
            var newPosition = new PlayerPosition();
            newPosition.Deserialize(reader);
            ActUpdatePosition?.Invoke(playerNumber, newPosition);
        }

        private void SetBomb(BinaryReader reader)
        {
            var netObjId = Utils.ReadUInt16(reader);
            var ownerNumber = Utils.ReadByte(reader);
            var position = new Vector2Int();
            position.DeserializeAsByte(reader);
            ActSetBomb?.Invoke(netObjId, ownerNumber, position);
        }

        private void SetFire(BinaryReader reader)
        {
            var position = new Vector2Int();
            var fireData = new FireData();

            var netId = Utils.ReadUInt16(reader);
            var ownerNumber = Utils.ReadByte(reader);
            position.DeserializeAsByte(reader);
            fireData.Deserialize(reader);
            ActSetFire?.Invoke(netId, ownerNumber, position, fireData);
        }

        private void RemoveObject(BinaryReader reader)
        {
            var netId = Utils.ReadUInt16(reader);
            ActRemoveObject?.Invoke(netId);
        }
        
        private void UpdatePlayerInfo(BinaryReader reader)
        {
            var playerNumber = Utils.ReadByte(reader);
            var playerInfo = new PlayerInfo();
            playerInfo.Deserialize(reader);
            ActUpdatePlayerInfo?.Invoke(playerNumber, playerInfo);
        }

        private void ReplaceCell(BinaryReader reader)
        {
            var position = new Vector2Int();
            position.DeserializeAsByte(reader);
            var newCellValue = Utils.ReadByte(reader);
            ActReplaceMapCell?.Invoke(position, newCellValue);
        }

        private void CreateResource(BinaryReader reader)
        {
            var netId = Utils.ReadUInt16(reader);
            var position = new Vector2Int();
            position.DeserializeAsByte(reader);
            var resourceIndex = Utils.ReadByte(reader);
            ActCreateResource?.Invoke(netId, position, resourceIndex);
        }

        // private void PlayerDeath(BinaryReader reader)
        // {
        //     var playerNumber = Utils.ReadByte(reader);
        //     GAPlayerDeath?.Invoke(playerNumber);
        // }
    }
}