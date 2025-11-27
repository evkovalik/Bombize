using System.IO;

namespace ServerAPI
{
    public abstract class RequestWithHandler<T> : IClientRequest
    {
        protected T _onAnswer;
        public RequestWithHandler(T onAnswer)
        {
            _onAnswer = onAnswer;
        }
        public void Reset() => _onAnswer = default;
        public abstract SrvMsgType AnswerMsgType();
        public abstract void Execute(WebSocketConnection connection);
        public abstract void ProcessRawAnswer(BinaryReader reader);
    }
}