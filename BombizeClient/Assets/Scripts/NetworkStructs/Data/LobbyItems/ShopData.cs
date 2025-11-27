using System.Collections.Generic;
using System.IO;
using GameNetwork.Interfaces;
using GameNetwork.Utilities;

namespace GameNetwork.NetworkData
{
    public class ShopData : IDataFromServer
    {
        public struct CharacterItem
        {
            public byte id;
            public uint price1;
            public uint price2;
        }

        private List<CharacterItem> _items;
        public IReadOnlyList<CharacterItem> Items { get => _items; }

        public void Deserialize(BinaryReader reader)
        {
            var count = Utils.ReadByte(reader);
            _items = new(count);
            
            for (int i = 0; i < count; i++)
            {
                var item = new CharacterItem
                {
                    id = Utils.ReadByte(reader),
                    price1 = Utils.ReadUInt32(reader),
                    price2 = Utils.ReadUInt32(reader)
                };
                _items.Add(item);
            }
        }
    }
}