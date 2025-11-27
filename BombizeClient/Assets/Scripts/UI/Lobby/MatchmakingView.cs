using UnityEngine;

public class MatchmakingView : MonoBehaviour
{
    [SerializeField] private GameObject _content;

    public void Show()
    {
        _content.SetActive(true);
        // run animation...
    }

    public void Hide()
    {
        _content.SetActive(false);
    }
}