using UnityEngine;
using System.Collections.Generic;
using UnityEngine.UI;
using GameNetwork.NetworkData;
using System.Threading.Tasks;

public class CharacterStorageView : MonoBehaviour
{
    [SerializeField] private Lobby _lobby; // It can be replaced with a personal controller
    [SerializeField] private GameObject _content;
    [SerializeField] private GameObject _itemPrefab;
    [SerializeField] private GameObject _emptyItemPrefab;
    [SerializeField] private RectTransform _itemsSpace;
    [SerializeField] private Button _btnNext;
    [SerializeField] private Button _btnBack;

    private List<GameObject> _items;
    private int _currentPage;
    private int _itemsCount;
    private UserInfo _userInfo;
    private const int ItemsInSpace = 3;
    private const float ItemWidth = 420f;
    private const float ItemSpacing = 20f;
    private const float ScrollFieldSize = -(ItemWidth + ItemSpacing) * ItemsInSpace;

    public void Init(UserInfo userInfo)
    {
        _userInfo = userInfo;
        var storage = Locator.GameStorage.Instance.Characters;
        if (_items == null)
        {
            _items = new(userInfo.Characters.Count + ItemsInSpace);
            _btnNext.onClick.AddListener(OnButtonNext);
            _btnBack.onClick.AddListener(OnButtonBack);
        }
        _itemsCount = 0;
        foreach (var character in userInfo.Characters)
        {
            var item = Instantiate(_itemPrefab, _itemsSpace);
            item.GetComponent<CharacterStorageItemView>().Init(
                character.Id,
                storage[character.Id].Name,
                storage[character.Id].Icon,
                character.GetTime(),
                OnSelect
            );
            if (userInfo.ActiveCharacter == character)
                item.GetComponent<CharacterStorageItemView>().Selectable = false;
            _items.Add(item);
            _itemsCount++;
        }
        int diff = _itemsCount % ItemsInSpace;
        if (diff != 0)
        {
            diff = ItemsInSpace - diff;
            for (int i = 0; i < diff; i++)
                _items.Add(Instantiate(_emptyItemPrefab, _itemsSpace));
            _itemsCount += diff;
        }
        _currentPage = 0;
        UpdateScrollButtons();
    }

    public void Refresh(UserInfo userInfo)
    {
        if (_items != null)
        {
            foreach (var item in _items) Destroy(item);
            _items.Clear();
        }
        Init(userInfo);
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
        for(int i = 0; i < _userInfo.Characters.Count; i++)
        {
            _items[i].GetComponent<CharacterStorageItemView>().UpdateTime(
                _userInfo.Characters[i].GetTime()
            );
        }
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

    private void OnSelect(int id)
    {
        int index = _userInfo.Characters.FindIndex(character => character.Id == id);
        Debug.Log($"Selected id: {id}, index = {index}");
        _lobby.SelectCharacter(index);
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
}
