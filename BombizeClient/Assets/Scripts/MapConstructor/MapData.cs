using System;
using System.Collections.Generic;
using UnityEngine;

namespace MapConstructor
{
    [Serializable]
    public class MapData
    {
        [SerializeField] private int width;
        [SerializeField] private int height;
        public int timeLifeSec;
        public int rateDestructionMS;
        public int background;
        [SerializeField] private int[] field; // index (0;0) = left down
        public List<Vector2Int> spawnPositions;

        public int Width { get => width; }
        public int Height { get => height; }
        public int[] Field { get => field; }

        public MapData(int width, int height)
        {
            this.width = width;
            this.height = height;
            field = new int[width * height];
            spawnPositions = new();
        }

        public void SetCellValue(Vector3Int position, int value)
        {
            field[position.x + position.y * width] = value;
        }

        public int GetCellValue(Vector3Int position)
        {
            return field[position.x + position.y * width];
        }

        public void ForEachCell(Action<Vector3Int, int> function)
        {
            for (int x = 0; x < width; x++)
                for (int y = 0; y < height; y++)
                    function(new(x, y), field[x + y * width]);
        }
    }
}