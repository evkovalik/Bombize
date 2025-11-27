using System;
using System.IO;
using GameNetwork.NetworkData;
using GameNetwork.Utilities;

namespace ServerAPI
{
    public class ReqGetUserInfo : RequestWithHandler<Action<Status, UserInfo>>
    {
        public ReqGetUserInfo(Action<Status, UserInfo> onAnswer)
        : base(onAnswer)
        {}
        public override SrvMsgType AnswerMsgType() => SrvMsgType.UserInfo;
        public override void Execute(WebSocketConnection connection)
        {
            connection.SendData(Utils.ConvertToBytes(CltMsgType.GetUserInfo));
        }
        public override void ProcessRawAnswer(BinaryReader reader)
        {
            UserInfo data = null;
            var status = Utils.ReadStatus(reader);
            if (status == Status.Ok)
            {
                data = new();
                data.Deserialize(reader);
            }
            _onAnswer?.Invoke(status, data);
        }
    }
}