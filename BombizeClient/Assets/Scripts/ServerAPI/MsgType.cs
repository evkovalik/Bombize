
namespace ServerAPI
{
    public enum Status : byte
    {
        None,
        Ok,
        Error
    }

    public enum SrvMsgType : ushort
    {
        Empty,
        Confirm,
        NewAccount,
        Account,
        UserInfo,
        RunGame,
        ResultGame,
        MAX
    }

    public enum CltMsgType: ushort
    {
        Empty,
        NewUser,
        Login,
        Quit,
        GetUserInfo,
        Play,
        CloseGameRoom,
        Buy,
        SetCharacter,
        MAX
    }
}
