using UnityEngine;
using TMPro;
using System;
using GameNetwork.NetworkData;

public class GameRewardView : MonoBehaviour
{
    [SerializeField] private GameObject _rewardContent;
    [SerializeField] private TMP_Text _textPlace;
    [SerializeField] private TMP_Text _textExperience;
    [SerializeField] private TMP_Text _textRating;
    [SerializeField] private TMP_Text _textCoins;

    private Action _onCloseReward = null;
    public void ShowReward(RewardGame reward, Action onCloseReward)
    {
        _onCloseReward = onCloseReward;
        _rewardContent.SetActive(true);
        _textPlace.text = reward.Place.ToString() + _textPlace.text[1..];

        _textExperience.text = GetRewardFormatForItem(reward.Experience);
        _textRating.text = GetRewardFormatForItem(reward.Rating);
        _textCoins.text = GetRewardFormatForItem(reward.Coins);
    }

    public void OnButtonTakeReward()
    {
        _onCloseReward?.Invoke();
        _onCloseReward = null;
    }

    private string GetRewardFormatForItem(int value)
    {
        return value > 0 ? '+' + value.ToString() : value.ToString();
    }
}
