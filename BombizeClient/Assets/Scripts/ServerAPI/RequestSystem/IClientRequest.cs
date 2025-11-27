using System.IO;

namespace ServerAPI
{
    public interface IClientRequest
    {
        public SrvMsgType AnswerMsgType();
        public void Execute(WebSocketConnection connection);
        public void ProcessRawAnswer(BinaryReader reader);
        public void Reset();
    }
}