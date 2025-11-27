using UnityEngine;
using GameNetwork.NetworkData;
using LibUIElements;
using Gameplay;

public class GameRoomView : MonoBehaviour
{
    [SerializeField] private GameRoom _gameRoom;
    [SerializeField] private DialogView _exitDialogView;
    [SerializeField] private GameRewardView _gameRewardView;

    [SerializeField] private PlayerLifeView _playerLifeView;
    [SerializeField] private PlayerResourceView _resourceBombView;
    [SerializeField] private PlayerResourceView _resourceSpeedView;
    [SerializeField] private PlayerResourceView _resourceFireView;

    public void OnButtonExit()
    {
        _exitDialogView.Show("Покинуть матч?", HandleAnswerBtnExit); // temporary. Add message storage!
    }

    public void ShowReward(RewardGame rewardGame)
    {
        _gameRewardView.ShowReward(rewardGame, () => _gameRoom.OpenLobby());
    }

    private void HandleAnswerBtnExit(bool result)
    {
        if (result)
        {
            _gameRoom.Close();
        }
        _exitDialogView.Hide();
    }

    public void UpdatePlayerInfo(PlayerInfo playerInfo)
    {
        _playerLifeView.Value = playerInfo.Health;
        _resourceBombView.Value = playerInfo.Bombs;
        _resourceSpeedView.Value = playerInfo.Speed.AsInt;
        _resourceFireView.Value = playerInfo.Fire;
    }
}
