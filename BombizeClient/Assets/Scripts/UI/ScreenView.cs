using UnityEngine;
using TMPro;

public class ScreenView : MonoBehaviour
{
    [SerializeField] private GameObject _screen;
    [SerializeField] private GameObject _waitingPanel;
    [SerializeField] private TMP_Text _waitingText;

    public void SetLockPanel(string text)
    {
        _waitingText.text = text;
        _waitingPanel.SetActive(true);
    }

    public void ResetLockPanel()
    {
        _waitingPanel.SetActive(false);
    }

    public void FreeScreen()
    {
        _screen.SetActive(false);
    }

    public void LoadingScreen()
    {
        _screen.SetActive(true);
    }

    public void LobbyLoadingScreen()
    {
        _screen.SetActive(true);
    }

    public void GameRoomLoadingScreen()
    {
        _screen.SetActive(true);
    }
}
