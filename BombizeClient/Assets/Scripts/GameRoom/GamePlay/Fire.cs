using UnityEngine;

namespace Gameplay
{
    public class Fire : MonoBehaviour
    {
        [SerializeField] private GameObject _ox;
        [SerializeField] private GameObject _oy;

        public void SetSize(Vector2 scaleXY)
        {
            _ox.transform.localScale = new Vector2(scaleXY.x, 1);
            _oy.transform.localScale = new Vector2(1, scaleXY.y);
        }

        public void SetOffset(Vector2 offsetXY)
        {
            _ox.transform.localPosition = new Vector2(offsetXY.x, 0);
            _oy.transform.localPosition = new Vector2(0, offsetXY.y);
        }

        public void SetSizeAndOffset(Vector2 scaleXY, Vector2 offsetXY)
        {
            SetSize(scaleXY);
            SetOffset(offsetXY);
        }
    }
}