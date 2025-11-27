using UnityEngine;
using TMPro;
using System;
using UnityEngine.UI;

namespace LibUIElements
{
    public class PurchaseConfirmView : MonoBehaviour
    {
        [SerializeField] private GameObject _content;
        [SerializeField] private TMP_Text _dialogText;

        [SerializeField] private TMP_Text _finalPriceText;
        [SerializeField] private Image _finalPriceIcon;
        [SerializeField] private Sprite _spritePriceIcon1;
        [SerializeField] private Sprite _spritePriceIcon2;

        private Action<bool> _onAnswer;
        public void Show(string dialogText, Action<bool> onAnswer)
        {
            _dialogText.text = dialogText;
            _content.SetActive(true);
            _onAnswer = onAnswer;
        }

        public void Hide()
        {
            _content.SetActive(false);
        }

        public void OnButtonYes()
        {
            _onAnswer?.Invoke(true);
            _onAnswer = null;
        }
        public void OnButtonNo()
        {
            _onAnswer?.Invoke(false);
            _onAnswer = null;
        }
    }
}