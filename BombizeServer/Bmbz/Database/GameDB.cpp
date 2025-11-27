#include "GameDB.hpp"
#include <string_view>
#include <format>

namespace Bmbz
{
    struct Column
    {
        const int i;
        const char* s;
    };
    static constexpr Column CL_Id               {0, "id"};
    static constexpr Column CL_Login            {1, "login"};
    static constexpr Column CL_Password         {2, "password"};
    static constexpr Column CL_Nickname         {3, "nickname"};
    static constexpr Column CL_Experience       {4, "experience"};
    static constexpr Column CL_Rating           {5, "rating"};
    static constexpr Column CL_CoinsC           {6, "coinsC"};
    static constexpr Column CL_CoinsQ           {7, "coinsQ"};
    static constexpr Column CL_CharacterIndex   {8, "characterIndex"};
    static constexpr Column CL_Characters       {9, "characters"};

    GameDB::GameDB(Config::DBConfig config)
    : _config(std::move(config))
    , _pool(_config.threadsCount)
    {}

    GameDB::~GameDB()
    {
        _pool.join();
    }

    void GameDB::AsyncGetUserData(User::LoginData data, OnGetUserData handler)
    {
        auto blockingTask = [this, handler, data = std::move(data)]
        ()
        {
            handler(RequesUserInfo(data));
        };
        boost::asio::post(_pool, blockingTask);
    }

    void GameDB::AsyncUpdateUserData(std::shared_ptr<User::UserInfo> userInfo)
    {
        auto blockingTask = [this, userInfo]
        ()
        {
            std::string jsonCharactersArray("[");
            for(const auto& character : userInfo->characters)
            {
                std::string jsonCharacterObj = std::format(
                    R"({{"id": {}, "time": {}}})",
                    character.id,
                    character.GetDisappearanceTimeAsInt64()
                );
                jsonCharactersArray += jsonCharacterObj + ",";
            }
            jsonCharactersArray.back() = ']';

            try
            {
                GetTable().update()
                    .set(CL_Nickname.s, userInfo->nickname.Content())
                    .set(CL_Experience.s, userInfo->experience)
                    .set(CL_Rating.s, userInfo->rating)
                    .set(CL_CoinsC.s, userInfo->coinsC)
                    .set(CL_CoinsQ.s, userInfo->coinsQ)
                    .set(CL_CharacterIndex.s, userInfo->activeCharacterIndex)
                    .set(CL_Characters.s, jsonCharactersArray)
                    .where("id = :id")
                    .bind("id", userInfo->id)
                    .execute();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        };
        boost::asio::post(_pool, blockingTask);
    }

    void GameDB::AsyncCreateUser(OnCreatedUser handler)
    {
        auto blockingTask = [this, handler = std::move(handler)]
        ()
        {
            User::LoginData loginData;
            std::shared_ptr<User::UserInfo> userInfo = nullptr;
            int attempts = 10;
            do
            {
                loginData.login = _genString.GetString(_loginSize);
                if(--attempts <= 0)
                {
                    // error creating login...
                    std::move(handler)(std::move(loginData), nullptr);
                    return;
                }
            } while (CheckLoginInBD(loginData.login.Content()));

            loginData.password = _genString.GetString(_loginSize);
            try
            {
                auto result = GetTable()
                    .insert(
                        CL_Login.s, CL_Password.s, CL_Nickname.s,
                        CL_Experience.s, CL_Rating.s, CL_CoinsC.s,
                        CL_CoinsQ.s, CL_CharacterIndex.s, CL_Characters.s
                    )
                    .values(
                        loginData.login.Content(), loginData.password.Content(), "user", 
                        0, 0, 0,
                        0, 0, R"([{"id": 0, "time": 0}])"
                    )
                    .execute();
                userInfo = RequesUserInfo(loginData);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            std::move(handler)(std::move(loginData), userInfo);
        };
        boost::asio::post(_pool, blockingTask);
    }

    bool GameDB::CheckLoginInBD(const std::string& login)
    {
        mysqlx::RowResult result = GetTable()
            .select("*")
            .where("login = :login")
            .bind("login", login)
            .execute();
        return result.count() > 0;
    }

    std::shared_ptr<User::UserInfo> GameDB::RequesUserInfo(const User::LoginData& data)
    {
        mysqlx::RowResult result = GetTable()
            .select("*")
            .where("login = :login AND password = :password")
            .bind("login", data.login.Content())
            .bind("password", data.password.Content())
            .execute();

        std::shared_ptr<User::UserInfo> userInfo = nullptr;
        if (result.count() > 0)
        {
            mysqlx::Row row = result.fetchOne();
            userInfo = std::make_shared<User::UserInfo>();

            userInfo->id = row[CL_Id.i].get<int>();
            userInfo->nickname = std::move(row[CL_Nickname.i].get<std::string>());
            userInfo->experience = row[CL_Experience.i].get<int>();
            userInfo->rating = row[CL_Rating.i].get<int>();
            userInfo->coinsC = row[CL_CoinsC.i].get<int>();
            userInfo->coinsQ = row[CL_CoinsQ.i].get<int>();
            userInfo->activeCharacterIndex = row[CL_CharacterIndex.i].get<int>();
            
            userInfo->characters.reserve(row[CL_Characters.i].elementCount());
            for(const auto& characterDoc : row[CL_Characters.i])
            {
                userInfo->characters.emplace_back(
                    User::Character(characterDoc["id"], characterDoc["time"])
                );
            }
        }
        return userInfo;
    }

    // REMOVE
    // usersTable.remove().where("login = :login").bind("login", "user1").execute();
}