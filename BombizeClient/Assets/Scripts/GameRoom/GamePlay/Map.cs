using System.Collections.Generic;
using GameNetwork.NetworkData;
using UnityEngine;
using UnityEngine.Tilemaps;

namespace Gameplay
{
    public class Map : MonoBehaviour
    {
        [SerializeField] private Tilemap _tilemapField;
        [SerializeField] private SpriteRenderer _background;
        private IReadOnlyList<Tile> _blocks;
        private const int EmptyCellValue = 0;

        public void Init(MapInfo mapInfo)
        {
            _blocks = Locator.GameStorage.Instance.MapBlocks;
            var cellValues = mapInfo.Field;

            for (int x = 0; x < mapInfo.Width; x++)
            {
                for (int y = 0; y < mapInfo.Height; y++)
                {
                    var blockId = cellValues[x + y * mapInfo.Width];
                    if (blockId == EmptyCellValue)
                        _tilemapField.SetTile(new(x, y), null);
                    else
                        _tilemapField.SetTile(new(x, y), _blocks[blockId]);
                }
            }
            _background.sprite = Locator.GameStorage.Instance.MapBackgrounds[mapInfo.Background];
        }

        public void ReplaceCell(Vector2Int position, int value)
        {
            if (value == EmptyCellValue)
            {
                _tilemapField.SetTile((Vector3Int)position, null);
            }
            else
            {
                _tilemapField.SetTile((Vector3Int)position, _blocks[value]);
            }
        }

        public Vector3 ToWorldPosition(Vector2Int position)
        {
            return _tilemapField.CellToWorld((Vector3Int)position);
        }
    }
}