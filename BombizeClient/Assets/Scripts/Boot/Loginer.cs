using UnityEngine;
using ServerAPI;
using GameNetwork.NetworkData;
using GameNetwork.Interfaces;
using System;

class Loginer
{
    public void Login(RemoteServer server, Action<bool, AccountData> onResult)
    {
        var loginData = new LoginDataDefault();
        if(loginData.Load())
        {
            server.Request(new ReqGetAccount(
                loginData,
                (Status status, AccountData data) => 
                {
                    if (status == Status.Ok)
                    {
                        onResult?.Invoke(true, data);
                    }
                    else
                    {
                        Debug.Log($"Error::ReqGetAccount");
                        onResult?.Invoke(false, null);
                    }
                }
            ));
        }
        else
        {
            server.Request(new ReqGetNewAccount(
                (Status status, ILoginData login, AccountData data) => 
                {
                    if (status == Status.Ok)
                    {
                        login.Save();
                        onResult?.Invoke(true, data);
                    }
                    else
                    {
                        Debug.Log($"Error::ReqGetNewAccount");
                        onResult?.Invoke(false, null);
                    }
                }
            ));
        }
    }
}