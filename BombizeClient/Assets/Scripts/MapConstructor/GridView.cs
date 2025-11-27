using UnityEngine;

namespace MapConstructor
{
    public class GridView : MonoBehaviour
    {
        [SerializeField] private Color LineColor = Color.gray;
        private float _lineWidth = 0.02f;
        private Transform _gridTransform;
        private int _startX = -9;
        private int _sizeX = 21;
        private int _startY = -7;
        private int _sizeY = 11;
        private bool _created = false;
        private bool _enabled = false;
        private Material _material;

        void Awake()
        {
            _gridTransform = new GameObject("Grid").transform;
            _gridTransform.SetParent(transform);

            _material = new Material(Shader.Find("Sprites/Default"));
            Show();
        }

        public void Show()
        {
            if (!_created)
            {
                CreateGrid();
                _created = true;
            }
            else
                _gridTransform.gameObject.SetActive(true);
            _enabled = true;
        }

        public void Hide()
        {
            _gridTransform.gameObject.SetActive(false);
            _enabled = false;
        }

        public void Swith()
        {
            if (_enabled) Hide();
            else Show();
        }

        private void CreateGrid()
        {
            for (int ox = _startX; ox < _startX + _sizeX; ox++)
                CreateLine(new(ox, _startY), new(ox, _startY + _sizeY));

            for (int oy = _startY; oy < _startY + _sizeY; oy++)
                CreateLine(new(_startX, oy), new(_startX + _sizeX, oy));
        }

        private void CreateLine(Vector2 startPos, Vector2 endPos)
        {
            var lineObj = new GameObject("Line");
            lineObj.transform.SetParent(_gridTransform);
            var line = lineObj.AddComponent<LineRenderer>();
            line.material = _material;

            line.startWidth = _lineWidth;
            line.endWidth = _lineWidth;
            line.startColor = LineColor;
            line.endColor = LineColor;

            line.SetPosition(0, startPos);
            line.SetPosition(1, endPos);
        }
    }
}