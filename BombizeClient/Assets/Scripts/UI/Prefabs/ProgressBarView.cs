using UnityEngine;
using UnityEngine.UI;

namespace LibUIElements
{
    public class ProgressBarView : MonoBehaviour
    {
        [SerializeField] private Image _progressImage;
        public void Value(float value)
        {
            value = value <= 0 ? 0 : (value >= 1 ? 1 : value);
            _progressImage.fillAmount = value;
        }
    }   
}
