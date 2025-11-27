#ifndef BMBZ_GAMEDB_HPP
#define BMBZ_GAMEDB_HPP

#include <mysqlx/xdevapi.h>
#include <boost/asio.hpp>
#include <thread>
#include "Config/DBConfig.hpp"
#include "User/LoginData.hpp"
#include "User/UserInfo.hpp"
#include "Generators/GenStrings.hpp"

namespace Bmbz
{
    class GameDB
    {
    public:
        using OnGetUserData = std::function<void(std::shared_ptr<User::UserInfo>)>;
        using OnCreatedUser = std::function<void(User::LoginData, std::shared_ptr<User::UserInfo>)>;

        GameDB(Config::DBConfig config);
        ~GameDB();

        void AsyncGetUserData(User::LoginData data, OnGetUserData handler);
        void AsyncUpdateUserData(std::shared_ptr<User::UserInfo> userInfo);
        void AsyncCreateUser(OnCreatedUser handler);
        // void AsyncDeleteUser(int userId);

    private:
        Config::DBConfig _config;
        boost::asio::thread_pool _pool;
        Generator::GenStrings _genString;
        static constexpr int _loginSize = 16;

        mysqlx::Table& GetTable()
        {
            thread_local mysqlx::Session session(
                _config.host, _config.port, _config.login, _config.password, _config.dbName
            );
            thread_local mysqlx::Schema schema = session.getSchema(_config.dbName);
            thread_local mysqlx::Table table = schema.getTable(_config.tableName);
            return table;
        }

        bool CheckLoginInBD(const std::string& login);
        std::shared_ptr<User::UserInfo> RequesUserInfo(const User::LoginData& data);
    };
}
#endif