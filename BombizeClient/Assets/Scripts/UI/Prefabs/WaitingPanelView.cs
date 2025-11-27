using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace LibUIElements
{
    public class WaitingPanelView : MonoBehaviour
    {
        [SerializeField] private GameObject _content;
        [SerializeField] private List<GameObject> _dynamicElements;
        private WaitForSeconds _waitTime = new(.2f);
        private Coroutine _coroutine;

        public void Show()
        {
            _content.SetActive(true);
            if (_coroutine != null)
                StopCoroutine(_coroutine);
            _coroutine = StartCoroutine(WaitingAnimation());
        }

        public void Hide()
        {
            if (_coroutine != null)
            {
                StopCoroutine(_coroutine);
                _coroutine = null;
            }
            _content.SetActive(false);
        }

        IEnumerator WaitingAnimation()
        {
            int index = 0;
            while (true)
            {
                if(index >= _dynamicElements.Count) index = 0;

                foreach (var item in _dynamicElements)
                    item.SetActive(false);
                    
                _dynamicElements[index].SetActive(true);
                index++;
                yield return _waitTime;
            }
        }
    }   
}
