#ifndef BMBZ_GAME_ROOMDATA_HPP
#define BMBZ_GAME_ROOMDATA_HPP

#include "Utils/ReadWriteData.hpp"
#include "User/UserInfo.hpp"
#include "PlayerSettings.hpp"
#include "Resources/Maps/MapInfo.hpp"

namespace Bmbz
{
namespace Game
{
namespace NetworkData
{
	class RoomData
	{
	public:
		RoomData(const Resource::MapInfo& mapInfo) : _mapInfo(mapInfo) {}

		PlayerSettings playerSettings;

		void AddPlayerData(uint8_t playerNumber, std::shared_ptr<User::UserInfo> userInfo)
		{
			_players.emplace_back(playerNumber, userInfo);
		}

		void Serialize(boost::beast::flat_buffer& buffer) const
		{
			playerSettings.Serialize(buffer);
			
			Utils::WriteToBuffer(buffer, static_cast<uint8_t>(_players.size()));
			for(const auto& player : _players)
			{
				Utils::WriteToBuffer(buffer, player.first);
				player.second->Serialize(buffer);
			}
			_mapInfo.Serialize(buffer);
		}
		
	private:
		Resource::MapInfo _mapInfo;
		std::vector<std::pair<uint8_t, std::shared_ptr<User::UserInfo>>> _players;
	};
}
}
}
#endif