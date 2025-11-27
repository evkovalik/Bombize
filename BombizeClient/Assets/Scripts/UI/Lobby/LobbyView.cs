using UnityEngine;
using TMPro;
using GameNetwork.NetworkData;
using LibUIElements;

public class LobbyView : MonoBehaviour
{
    [SerializeField] private Lobby _lobby;
    [SerializeField] private TMP_Text _textNickname;
    [SerializeField] private TMP_Text _textRating;
    [SerializeField] private TMP_Text _textCoins1;
    [SerializeField] private TMP_Text _textCoins2;
    [SerializeField] private TMP_Text _textLevel;
    [SerializeField] private ProgressBarView _levelProgressBar;
    [SerializeField] private MatchmakingView _matchmakingView;

    private CharacterInLobby _characterInLobby = new();

    public void UpdateUserInfo(UserInfo userInfo)
    {
        _textNickname.text = userInfo.Nickname.Content;
        _textRating.text = userInfo.Rating.ToString();
        _textCoins1.text = userInfo.CoinsC.ToString();
        _textCoins2.text = userInfo.CoinsQ.ToString();
        _textLevel.text = userInfo.Level.ToString();

        float progress = (float)userInfo.ExperiencePoints / userInfo.GetMaxExperienceForLevel();
        _levelProgressBar.Value(progress);

        _characterInLobby.CreateOrUpdate(userInfo.ActiveCharacter.Id);
    }

    public void OnButtonPlay()
    {
        _lobby.ToGameRoom();
    }

    public void OnButtonCancelGame()
    {
        _lobby.CancelToGameRoom();
    }

    public void SetModeGameWaiting() => _matchmakingView.Show();
    public void ResetModeGameWaiting() => _matchmakingView.Hide();
}
