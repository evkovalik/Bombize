using UnityEngine;
using TMPro;
using UnityEngine.UI;
using System;

public class ShopItemView : MonoBehaviour
{
    [SerializeField] private TMP_Text _name;
    [SerializeField] private Image _icon;
    [SerializeField] private TMP_Text _textPrice1;
    [SerializeField] private TMP_Text _textPrice2;
    private int _id;
    private Action<int, Currency> _onBuy;
    private Currency _price1;
    private Currency _price2;

    public void Init(
        int id,
        string name,
        Sprite sprite,
        Currency price1,
        Currency price2,
        Action<int, Currency> onBuy)
    {
        _id = id;
        _onBuy = onBuy;
        _price1 = price1;
        _price2 = price2;

        _name.text = name;
        _icon.sprite = sprite;
        _textPrice1.text = price1.value.ToString();
        _textPrice2.text = price2.value.ToString();
    }

    public void OnButtonBuy1()
    {
        _onBuy?.Invoke(_id, _price1);
    }

    public void OnButtonBuy2()
    {
        _onBuy?.Invoke(_id, _price2);
    }
}
