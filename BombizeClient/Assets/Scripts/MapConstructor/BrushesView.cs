using UnityEngine;

namespace MapConstructor
{
    public class BrushesView : MonoBehaviour
    {
        [SerializeField] private ConstructorView _mapConstructorView;
        [SerializeField] private GameObject _content;
        [SerializeField] private Transform _itemsSpace;
        [SerializeField] private GameObject _itemPrefab;

        public void Show() => _content.SetActive(true);
        public void Hide() => _content.SetActive(false);

        private void Awake()
        {
            var storage = Locator.GameStorage.Instance.MapBlocks;
            for (int index = 1; index < storage.Count; index++)
            {
                var itemView = Instantiate(_itemPrefab, _itemsSpace).GetComponent<ItemView>();
                itemView.Init(index, storage[index].sprite, OnSelectPen);
            }
        }

        private void OnSelectPen(int id)
        {
            _mapConstructorView.SelectPen(id);
            Hide();
        }
    }
}