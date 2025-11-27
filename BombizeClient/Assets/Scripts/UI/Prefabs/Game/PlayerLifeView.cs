using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerLifeView : MonoBehaviour
{
    [SerializeField] private List<Image> _items;
    private int _value = 3;
    public int Value
    {
        get => _value;
        set
        {
            if (value == _value) return;
            value = value < 0 ? 0 : (value > _items.Count ? _items.Count : value);
            int currentHealth = 0;
            foreach (var item in _items)
            {
                if (currentHealth < value) item.enabled = true;
                else item.enabled = false;
                currentHealth++;
            }
            _value = value;
        }
    }
}
