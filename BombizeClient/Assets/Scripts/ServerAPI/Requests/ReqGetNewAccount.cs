using System;
using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.NetworkData;
using GameNetwork.Utilities;

namespace ServerAPI
{
    /* Default login method */
    public class ReqGetNewAccount : RequestWithHandler<Action<Status, ILoginData, AccountData>>
    {
        public ReqGetNewAccount(Action<Status, ILoginData, AccountData> onAnswer)
        : base(onAnswer)
        {}
        public override SrvMsgType AnswerMsgType() => SrvMsgType.NewAccount;
        public override void Execute(WebSocketConnection connection)
        {
            connection.SendData(Utils.ConvertToBytes(CltMsgType.NewUser));
        }
        public override void ProcessRawAnswer(BinaryReader reader)
        {
            ILoginData login = null;
            AccountData data = null;
            var status = Utils.ReadStatus(reader);
            if(status == Status.Ok)
            {
                login = new LoginDataDefault(); // temp
                data = new();
                login.Deserialize(reader);
                data.Deserialize(reader);
            }
            _onAnswer?.Invoke(status, login, data);
        }
    }
}