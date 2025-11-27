using UnityEngine;
using TMPro;
using System;
using UnityEngine.UI;

public class PurchaseConfirmView : MonoBehaviour
{
    [SerializeField] private GameObject _content;
    [SerializeField] private TMP_Text _priceText;
    [SerializeField] private Image _finalPriceIcon;
    [SerializeField] private Sprite _iconPrice1;
    [SerializeField] private Sprite _iconPrice2;
    private Action<bool> _callback;

    public void Show(Currency finalPrice, Action<bool> onAnswer)
    {
        if(finalPrice.type == CurrencyType.Coin1)
            _finalPriceIcon.sprite = _iconPrice1;
        else if(finalPrice.type == CurrencyType.Coin2)
            _finalPriceIcon.sprite = _iconPrice2;

        _priceText.text = finalPrice.value.ToString();
        _callback = onAnswer;
        _content.SetActive(true);
    }

    public void Hide()
    {
        _callback = null;
        _content.SetActive(false);
    }

    public void OnButtonYes()
    {
        _callback?.Invoke(true);
    }
    public void OnButtonNo()
    {
        _callback?.Invoke(false);
    }
}
