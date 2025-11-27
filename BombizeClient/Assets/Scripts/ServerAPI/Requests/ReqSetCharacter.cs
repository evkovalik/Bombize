using System;
using System.IO;
using GameNetwork.NetworkData;
using GameNetwork.Utilities;

namespace ServerAPI
{
    public class ReqSetCharacter : RequestWithHandler<Action<Status, UserInfo>>
    {
        byte _index;
        public ReqSetCharacter(byte index, Action<Status, UserInfo> onAnswer)
        : base(onAnswer)
        {
            _index = index;
        }
        public override SrvMsgType AnswerMsgType() => SrvMsgType.UserInfo;
        public override void Execute(WebSocketConnection connection)
        {
            var outStream = new MemoryStream();
            Utils.WriteToStream(outStream, CltMsgType.SetCharacter);
            Utils.WriteToStream(outStream, _index);
            connection.SendData(outStream);
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