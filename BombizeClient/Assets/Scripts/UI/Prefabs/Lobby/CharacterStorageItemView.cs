using UnityEngine;
using TMPro;
using UnityEngine.UI;
using System;

public class CharacterStorageItemView : MonoBehaviour
{
    [SerializeField] private TMP_Text _name;
    [SerializeField] private Image _icon;
    [SerializeField] private TMP_Text _textDisappearanceTime;
    [SerializeField] private Button _selectButton;
    private int _id;
    public bool Selectable
    {
        set => _selectButton.interactable = value;
    }
    private Action<int> _onSelect;

    public void Init(
        int id,
        string name,
        Sprite sprite,
        DateTime? disappearanceTime,
        Action<int> onSelect)
    {
        _id = id;
        _name.text = name;
        _icon.sprite = sprite;
        _onSelect = onSelect;
        Selectable = true;
        UpdateTime(disappearanceTime);
    }

    public void UpdateTime(DateTime? disappearanceTime)
    {
        if (!disappearanceTime.HasValue)
            _textDisappearanceTime.text = "";
        else
        {
            TimeSpan difference = disappearanceTime.Value - DateTime.UtcNow;
            if (difference.TotalSeconds < 0)
                _textDisappearanceTime.text = "";
            else if (difference.TotalDays >= 1)
                _textDisappearanceTime.text = $"{difference.Days}дн {difference.Hours}ч";
            else if (difference.TotalHours >= 1)
                _textDisappearanceTime.text = $"{difference.Hours}ч {difference.Minutes}мин";
            else
                _textDisappearanceTime.text = $"{difference.Minutes}мин";
        }
    }

    public void OnButtonSelect() => _onSelect?.Invoke(_id);
}
