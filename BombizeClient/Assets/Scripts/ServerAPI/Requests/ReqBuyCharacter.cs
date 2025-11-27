using System;
using System.IO;
using GameNetwork.NetworkData;
using GameNetwork.Utilities;

namespace ServerAPI
{
    public class ReqBuyCharacter : RequestWithHandler<Action<Status, UserInfo>>
    {
        byte _id;
        private CurrencyType _currencyType;
        public ReqBuyCharacter(byte id, CurrencyType currencyType, Action<Status, UserInfo> onAnswer)
        : base(onAnswer)
        {
            _id = id;
            _currencyType = currencyType;
        }
        public override SrvMsgType AnswerMsgType() => SrvMsgType.UserInfo;
        public override void Execute(WebSocketConnection connection)
        {
            var outStream = new MemoryStream();
            Utils.WriteToStream(outStream, CltMsgType.Buy);
            Utils.WriteToStream(outStream, _id);
            Utils.WriteToStream(outStream, (byte)_currencyType);
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