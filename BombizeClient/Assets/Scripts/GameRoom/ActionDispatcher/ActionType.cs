
namespace GameNetwork.GameActions
{
    public enum ActionType : byte
    {
        Empty,
        Confirm,
        Refuse,
        Ping,
        CreateAvatar,
        UpdatePosition,
        UpdatePlayerInfo,
        SetBomb,
        SetFire,
        RemoveObject,
        CreateResource,
        ReplaceCell,
        PlayerDeath,
        MAX
    }
}