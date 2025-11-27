using System.IO;
using System.Text;
using GameNetwork.Types;
using GameNetwork.Utilities;

namespace GameNetwork.GameActions
{
    public partial class ActionDispatcher
    {
        // ----- Actins from client to server ----- //

        public void ConfirmActivity()
        {
            _deliverer.RepeatSend();
            
            var outMStream = _deliverer.GetCurrentOutStream();
            Utils.WriteToStream(outMStream, ActionType.Empty);
            _deliverer.SendCurrentData();
        }

        public void UpdatePosition(Vector2Fixed16Bit position, FloatAsUInt label)
        {
            var outMStream = _deliverer.GetCurrentOutStream();
            using var writer = new BinaryWriter(outMStream, Encoding.UTF8, true);

            Utils.WriteAsBinData(writer, ActionType.UpdatePosition);
            position.Serialize(writer);
            label.Serialize(writer);
            _deliverer.SendCurrentData();
        }

        public void TrySetBomb()
        {
            Utils.WriteToStream(_deliverer.GetCurrentOutStream(), ActionType.SetBomb);
            _deliverer.SetConfirmOfCurrentPackage();
            _deliverer.SendCurrentData();
        }
    }
}