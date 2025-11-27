using UnityEngine;
using TMPro;

public class PlayerResourceView : MonoBehaviour
{
    [SerializeField] private TMP_Text _textValue;

    private int _value = 0;
    public int Value
    {
        get => _value;
        set
        {
            _value = value;
            _textValue.text = value.ToString();
        }
    }
}
