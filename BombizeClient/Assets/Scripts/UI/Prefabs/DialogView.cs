using UnityEngine;
using TMPro;
using System;

namespace LibUIElements
{
    public class DialogView : MonoBehaviour
    {
        [SerializeField] private GameObject _content;
        [SerializeField] private TMP_Text _headerText;
        [SerializeField] private TMP_Text _msgText;
        private Action<bool> _callback;

        public void Show(string header, Action<bool> onAnswer)
        {
            _headerText.text = header;
            _msgText.text = "";
            _callback = onAnswer;
            _content.SetActive(true);
        }

        public void Show(string header, string msg, Action<bool> onAnswer)
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
}