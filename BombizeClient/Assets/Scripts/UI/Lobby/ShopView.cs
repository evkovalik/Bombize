using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using GameNetwork.NetworkData;

public class ShopView : MonoBehaviour
{
    [SerializeField] private Lobby _lobby; // can be replaced with a store controller
    [SerializeField] private GameObject _content;
    [SerializeField] private GameObject _itemPrefab;
    [SerializeField] private GameObject _emptyItemPrefab;
    [SerializeField] private RectTransform _itemsSpace;
    [SerializeField] private Button _btnNext;
    [SerializeField] private Button _btnBack;
    [SerializeField] private PurchaseConfirmView _purchaseConfirmView;

    private List<GameObject> _items;
    private int _currentPage;
    private int _itemsCount;
    private const int ItemsInSpace = 3;
    private const float ItemWidth = 420f;
    private const float ItemSpacing = 20f;
    private const float ScrollFieldSize = -(ItemWidth + ItemSpacing) * ItemsInSpace;

    public void Init(ShopData shopData)
    {
        var storage = Locator.GameStorage.Instance.Characters;
        if (_items == null)
        {
            _items = new(shopData.Items.Count + ItemsInSpace);
            _btnNext.onClick.AddListener(OnButtonNext);
            _btnBack.onClick.AddListener(OnButtonBack);
        }
        _itemsCount = 0;
        foreach (var shopItem in shopData.Items)
        {
            var item = Instantiate(_itemPrefab, _itemsSpace);
            item.GetComponent<ShopItemView>().Init(
                shopItem.id,
                storage[shopItem.id].Name,
                storage[shopItem.id].Icon,
                new Currency(CurrencyType.Coin1, shopItem.price1),
                new Currency(CurrencyType.Coin2, shopItem.price2),
                OnBuy
            );
            _items.Add(item);
            _itemsCount++;
        }
        int diff = _itemsCount % ItemsInSpace;
        if (diff != 0)
        {
            diff = ItemsInSpace - diff;
            for (int i = 0; i < diff; i++)
                Instantiate(_emptyItemPrefab, _itemsSpace);
            _itemsCount += diff;
        }
        _currentPage = 0;
        UpdateScrollButtons();
    }

    void OnDestroy()
    {
        _btnNext.onClick.RemoveListener(OnButtonNext);
        _btnBack.onClick.RemoveListener(OnButtonBack);
    }

    public void Show()
    {
        _currentPage = 0;
        UpdateScrollButtons();
        _itemsSpace.anchoredPosition = new(0, _itemsSpace.anchoredPosition.y);
        _content.SetActive(true);
    }

    public void Hide()
    {
        _content.SetActive(false);
    }

    public void OnButtonNext()
    {
        _currentPage++;
        _itemsSpace.anchoredPosition = new(ScrollFieldSize * _currentPage, _itemsSpace.anchoredPosition.y);
        UpdateScrollButtons();
    }

    public void OnButtonBack()
    {
        _currentPage--;
        _itemsSpace.anchoredPosition = new(ScrollFieldSize * _currentPage, _itemsSpace.anchoredPosition.y);
        UpdateScrollButtons();
    }

    private void UpdateScrollButtons()
    {
        if (_currentPage > 0)
            _btnBack.gameObject.SetActive(true);
        else
            _btnBack.gameObject.SetActive(false);

        if (_currentPage < (_itemsCount / ItemsInSpace) - 1)
            _btnNext.gameObject.SetActive(true);
        else
            _btnNext.gameObject.SetActive(false);
    }

    private void OnBuy(int id, Currency price)
    {
        _purchaseConfirmView.Show(price, (ok) =>
        {
            if(ok) _lobby.BuyCharacter(id, price.type);
            _purchaseConfirmView.Hide();
        });
    }
}
