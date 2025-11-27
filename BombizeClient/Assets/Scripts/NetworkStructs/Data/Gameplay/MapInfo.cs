using System.Collections.Generic;
using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Utilities;

namespace GameNetwork.NetworkData
{
    public class MapInfo : IDataFromServer
    {
        public int Background { get; private set; }
        public int Width { get; private set; }
        public int Height { get; private set; }
        private List<int> _field;
        public IReadOnlyList<int> Field { get => _field; }

        public void Deserialize(BinaryReader reader)
        {
            Background = Utils.ReadByte(reader);
            Width = Utils.ReadByte(reader);
            Height = Utils.ReadByte(reader);

            int count = Width * Height;
            _field = new(count);
            for (int i = 0; i < count; i++)
                _field.Add(Utils.ReadByte(reader));
        }
    }
}