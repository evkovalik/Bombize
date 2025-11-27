using System;
using System.IO;
using GameNetwork.Utilities;

namespace ServerAPI
{
    public class ReqTryAction : RequestWithHandler<Action<Status>>
    {
        // Requesting an action without receiving data from the server (except for the status)
        private CltMsgType _msgType;
        public ReqTryAction(CltMsgType msgType, Action<Status> onAnswer)
        : base(onAnswer)
        { _msgType = msgType; }
        public override SrvMsgType AnswerMsgType() => SrvMsgType.Confirm;
        public override void Execute(WebSocketConnection connection)
        {
            connection.SendData(Utils.ConvertToBytes(_msgType));
        }
        public override void ProcessRawAnswer(BinaryReader reader)
        {
            _onAnswer?.Invoke(Utils.ReadStatus(reader));
        }
    }
}