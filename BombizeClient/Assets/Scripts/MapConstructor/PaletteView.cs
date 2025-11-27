using System;
using System.Collections.Generic;
using UnityEngine;

namespace MapConstructor
{
    public class PaletteView : MonoBehaviour
    {
        [SerializeField] private GameObject _content;
        [SerializeField] private Transform _itemsSpace;
        [SerializeField] private GameObject _itemPrefab;
        private Action<int> _callback;

        public void Init(
            Action<int> onSelectItem,
            IReadOnlyList<Sprite> items,
            int startIndex = 0,
            int endIndex = 0)
        {
            _callback = onSelectItem;
            endIndex = endIndex <= 0 ? items.Count : endIndex;
            for (int index = startIndex; index < endIndex; index++)
            {
                var itemView = Instantiate(_itemPrefab, _itemsSpace).GetComponent<ItemView>();
                itemView.Init(index, items[index], OnSelectPen);
            }
        }
        public void Show() => _content.SetActive(true);
        public void Hide() => _content.SetActive(false);
        private void OnSelectPen(int id)
        {
            _callback?.Invoke(id);
            Hide();
        }
    }
}