using UnityEngine;
using TMPro;
using System;

namespace LibUIElements
{
    public class ServerMsgView : MonoBehaviour
    {
        [SerializeField] private GameObject _content;
        [SerializeField] private TMP_Text _headerText;
        [SerializeField] private TMP_Text _msgText;
        private Action _callback;
        
        public void Show(string header, string msg, Action onConfirm)
        {
            _headerText.text = header;
            _msgText.text = msg;
            _content.SetActive(true);
            _callback = onConfirm;
        }

        public void Hide()
        {
            _callback = null;
            _content.SetActive(false);
        }

        public void OnButtonOk()
        {
            _callback?.Invoke();
            //Hide();
        }
    }
}