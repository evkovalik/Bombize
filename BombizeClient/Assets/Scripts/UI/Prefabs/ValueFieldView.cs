using TMPro;
using UnityEngine;

namespace LibUIElements
{
    public class ValueFieldView : MonoBehaviour
    {
        [SerializeField] private TMP_Text _value;

        public void SetValue(string value)
        {
            _value.text = value;
        }
    }   
}
