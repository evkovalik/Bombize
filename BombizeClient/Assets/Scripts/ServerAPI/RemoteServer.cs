using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using GameNetwork.NetworkData;
using GameNetwork.Utilities;
using UnityEngine;
using WebSocketSharp;

namespace ServerAPI
{
    public class RemoteServer
    {
        public event Action<Status, GameRoomData> OnMsgRunGame;
        public event Action<Status, RewardGame> OnMsgRewardGame;
        private WebSocketConnection _connection;
        private List<IClientRequest> _requests = new();

        public RemoteServer()
        {
            _connection = new();
            _connection.OnMessage += HandleMessages;
        }

        ~RemoteServer()
        {
            _connection.OnMessage -= HandleMessages;
        }

        public void Request(IClientRequest request)
        {
            request.Execute(_connection);
            _requests.Add(request);
        }

        private void HandleMessages(MessageEventArgs messageArgs)
        {
            var inMStream = new MemoryStream(messageArgs.RawData, false);
            using var reader = new BinaryReader(inMStream, Encoding.UTF8, true);

            SrvMsgType msgType = Utils.ReadSrvMsgType(reader);
            Debug.Log($"Input message type: {msgType}");

            var request = _requests.Find(item => item.AnswerMsgType() == msgType);
            if(request != null)
            {
                request.ProcessRawAnswer(reader);
                request.Reset();
                _requests.Remove(request);
            }
            else
            {
                var status = Utils.ReadStatus(reader);
                if(msgType == SrvMsgType.RunGame)
                {
                    GameRoomData data = null;
                    if(status == Status.Ok)
                    {
                        data = new();
                        data.Deserialize(reader);
                    }
                    OnMsgRunGame?.Invoke(status, data);
                }
                else if(msgType == SrvMsgType.ResultGame)
                {
                    RewardGame data = null;
                    if(status == Status.Ok)
                    {
                        data = new();
                        data.Deserialize(reader);
                    }
                    OnMsgRewardGame?.Invoke(status, data);
                }
            }
        }
    }
}
