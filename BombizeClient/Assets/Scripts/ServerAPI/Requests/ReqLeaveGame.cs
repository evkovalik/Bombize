using System;
using System.IO;
using GameNetwork.NetworkData;
using GameNetwork.Utilities;

namespace ServerAPI
{
    public class ReqLeaveGame : RequestWithHandler<Action<Status, RewardGame>>
    {
        public ReqLeaveGame(Action<Status, RewardGame> onAnswer)
        : base(onAnswer)
        {}
        public override SrvMsgType AnswerMsgType() => SrvMsgType.ResultGame;
        public override void Execute(WebSocketConnection connection)
        {
            connection.SendData(Utils.ConvertToBytes(CltMsgType.CloseGameRoom));
        }
        public override void ProcessRawAnswer(BinaryReader reader)
        {
            RewardGame reward = null;
            var status = Utils.ReadStatus(reader);
            if (status == Status.Ok)
            {
                reward = new();
                reward.Deserialize(reader);
            }
            _onAnswer?.Invoke(status, reward);
        }
    }
}