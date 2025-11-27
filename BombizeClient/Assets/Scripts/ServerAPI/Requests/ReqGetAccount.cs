using System;
using System.IO;
using System.Text;
using GameNetwork.Interfaces;
using GameNetwork.NetworkData;
using GameNetwork.Utilities;

namespace ServerAPI
{
    public class ReqGetAccount : RequestWithHandler<Action<Status, AccountData>>
    {
        private ILoginData _loginData;
        public ReqGetAccount(ILoginData loginData, Action<Status, AccountData> onAnswer)
        : base(onAnswer)
        {
            _loginData = loginData;
        }
        public override SrvMsgType AnswerMsgType() => SrvMsgType.Account;
        public override void Execute(WebSocketConnection connection)
        {
            var outStream = new MemoryStream();
            Utils.WriteToStream(outStream, CltMsgType.Login);
            using var writer = new BinaryWriter(outStream, Encoding.UTF8, true);
            _loginData.Serialize(writer);
            connection.SendData(outStream);
        }
        public override void ProcessRawAnswer(BinaryReader reader)
        {
            AccountData data = null;
            var status = Utils.ReadStatus(reader);
            if(status == Status.Ok)
            {
                data = new();
                data.Deserialize(reader);
            }
            _onAnswer?.Invoke(status, data);
        }
    }
}