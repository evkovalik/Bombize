using UnityEngine;
using TMPro;
using System;

namespace LibUIElements
{
    public class InputDialogView : MonoBehaviour
    {
        [SerializeField] private GameObject _content;
        [SerializeField] private TMP_Text _headerText;
        [SerializeField] private TMP_Text _msgText;
        [SerializeField] private TMP_Text _inputText;
        private Action<bool, string> _callback;

        public void Show(string header, Action<bool, string> onAnswer)
        {
            _headerText.text = header;
            _msgText.text = "";
            _callback = onAnswer;
            _content.SetActive(true);
        }

        public void Show(string header, string msg, Action<bool, string> onAnswer)
        {
            _headerText.text = header;
            _msgText.text = msg;
            _callback = onAnswer;
            _content.SetActive(true);
        }

        public void Hide()
        {
            _callback = null;
            _content.SetActive(false);
            //_inputText.text = "";
        }

        public void OnButtonYes()
        {
            _callback?.Invoke(true, _inputText.text);
            Hide();
        }
        public void OnButtonNo()
        {
            _callback?.Invoke(false, null);
            Hide();
        }
    }
}