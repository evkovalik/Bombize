using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEngine.Tilemaps;

namespace MapConstructor
{
    public class Constructor : MonoBehaviour
    {
        [SerializeField] private Camera _camera;
        [SerializeField] private SpriteRenderer _background;
        [SerializeField] private Tilemap _tilemapPreview;
        [SerializeField] private Tilemap _tilemapDraw;
        [SerializeField] private Tile _tileEraser;
        [SerializeField] private Tile _tileSpawner;
        private IReadOnlyList<Tile> _blocks;
        private IReadOnlyList<Sprite> _backgrounds;
        private Vector3Int _offsetTilemap = new(-1, -1, 0);
        private MapData _mapData = new(21, 11);
        private Vector3Int _offsetArray = new(9, 7, 0);
        private const int EmptyCellValue = 0;
        private const int SpawnCellValue = -1;
        private Vector3Int _nowCellPos = Vector3Int.zero;
        private Vector3Int _oldCellPos = Vector3Int.zero;
        private bool _mouseOnCanvas = false;
        private int _penId;
        private Tile _pen;

        private enum Mode
        {
            Cursor,
            Pen,
            Eraser,
            Spawner
        }
        private Mode _mode = Mode.Cursor;

        public void SetCursor()
        {
            _mode = Mode.Cursor;
        }

        public void SetPen(int id)
        {
            _penId = id;
            _pen = _blocks[_penId];
            _mode = Mode.Pen;
        }

        public void SetEraser()
        {
            _penId = 0;
            _pen = _tileEraser; // preview
            _mode = Mode.Eraser;
        }

        public void SetSpawner()
        {
            _penId = 0;
            _pen = _tileSpawner;
            _mode = Mode.Spawner;
        }

        public void SetBackground(int id)
        {
            _background.sprite = _backgrounds[id];
            _mapData.background = id;
        }

        public void Save(string path)
        {
            _mapData.timeLifeSec = 30;
            _mapData.rateDestructionMS = 1000;

            _mapData.ForEachCell((cellPos, cellValue) =>
            {
                if (cellValue == SpawnCellValue)
                {
                    _mapData.spawnPositions.Add(new(cellPos.x, cellPos.y));
                    _mapData.SetCellValue(cellPos, EmptyCellValue);                    
                }
            });
            string json = JsonUtility.ToJson(_mapData, true);
            File.WriteAllText(path, json);
        }

        public void LoadFromFile(string path)
        {
            if (File.Exists(path))
            {
                var json = File.ReadAllText(path);
                _mapData = JsonUtility.FromJson<MapData>(json);
                DrawMapData(_mapData);
            }
            else Debug.Log($"Not found file: {path}");
        }

        private void Awake()
        {
            _backgrounds = Locator.GameStorage.Instance.MapBackgrounds;
            _blocks = Locator.GameStorage.Instance.MapBlocks;
            SetCursor();
        }

        private void Update()
        {
            if (_mode == Mode.Cursor) return;

            if (Input.GetMouseButton(0) && _mouseOnCanvas)
            {
                if (_mode == Mode.Pen)
                {
                    _tilemapDraw.SetTile(_nowCellPos, _pen);
                    _mapData.SetCellValue(_nowCellPos + _offsetArray, _penId);
                }
                else if (_mode == Mode.Eraser)
                {
                    _tilemapDraw.SetTile(_nowCellPos, null);
                    _mapData.SetCellValue(_nowCellPos + _offsetArray, EmptyCellValue);
                }
            }

            if (Input.GetMouseButtonDown(0) && _mouseOnCanvas)
            {
                if (_mode == Mode.Spawner)
                {
                    var cellPos = _nowCellPos + _offsetArray;
                    if (_mapData.GetCellValue(cellPos) == EmptyCellValue)
                    {
                        _tilemapDraw.SetTile(_nowCellPos, _pen);
                        _mapData.SetCellValue(cellPos, SpawnCellValue);
                    }
                }
            }
        }

        private void FixedUpdate()
        {
            // preview //
            if (_mode != Mode.Cursor)
            {
                Vector2 mousePosition = _camera.ScreenToWorldPoint(Input.mousePosition);
                RaycastHit2D hit = Physics2D.Raycast(mousePosition, Vector2.zero);

                if (hit.collider != null)
                {
                    _mouseOnCanvas = true;
                    _nowCellPos = new Vector3Int(
                        Mathf.CeilToInt(hit.point.x), Mathf.CeilToInt(hit.point.y)
                    ) + _offsetTilemap;

                    if (_oldCellPos != _nowCellPos)
                    {
                        _tilemapPreview.SetTile(_oldCellPos, null);
                        _tilemapPreview.SetTile(_nowCellPos, _pen);
                        _oldCellPos = _nowCellPos;
                    }
                }
                else
                {
                    _mouseOnCanvas = false;
                    _tilemapPreview.SetTile(_oldCellPos, null);
                }
            }
        }

        private void DrawMapData(MapData mapData)
        {
            mapData.ForEachCell((cellPos, cellValue) =>
            {
                if (cellValue == EmptyCellValue)
                    _tilemapDraw.SetTile(cellPos - _offsetArray, null);
                else
                    _tilemapDraw.SetTile(cellPos - _offsetArray, _blocks[cellValue]);
            });

            mapData.spawnPositions.ForEach(pos =>
            {
                _tilemapDraw.SetTile(new Vector3Int(pos.x, pos.y) - _offsetArray, _tileSpawner);
            });
            SetBackground(mapData.background);
        }
    }
}