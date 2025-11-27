#ifndef BMBZ_SERVER_MSGTYPE_HPP
#define BMBZ_SERVER_MSGTYPE_HPP

#include <cstdint>

namespace Bmbz 
{
namespace
{
    enum class MsgStatus : uint8_t
    {
        None,
        Ok,
        Error
    };

    enum class CltMsgType : uint16_t
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
    };

    enum class SrvMsgType : uint16_t
    {
        Empty,
        Confirm,
        NewAccount,
        Account,
        UserInfo,
        RunGame,
        ResultGame,
        MAX
    };
}
}

#endif