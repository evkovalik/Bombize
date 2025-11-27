using System;
using UnityEngine;
using UnityEngine.UI;

namespace MapConstructor
{
    public class ItemView : MonoBehaviour
    {
        [SerializeField] private Image _icon;
        private int _id;
        private Action<int> _callback;

        public void Init(int id, Sprite icon, Action<int> onSelect)
        {
            _id = id;
            _callback = onSelect;
            _icon.sprite = icon;
        }

        public void OnSelect() => _callback?.Invoke(_id);
    }

    
}