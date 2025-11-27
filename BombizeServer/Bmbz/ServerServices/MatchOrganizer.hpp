#ifndef BMBZ_MATCHORGANIZER_HPP
#define BMBZ_MATCHORGANIZER_HPP

#include <list>
#include "Game/GameRoom/RoomsManager.hpp"

namespace Bmbz
{
	class MatchOrganizer
	{
	public:
		MatchOrganizer() = delete;
		MatchOrganizer(boost::asio::io_context& ioc, Config::UdpConfig config)
		: _roomsManager(ioc, std::move(config))
		{}

		void RegisterForGame(std::shared_ptr<UserConnection> user)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_waitingUsers.push_back(user);

			// temp
			int countGamersForRun = 2; // Game::GroupToGame::CAPACITY
			if(_waitingUsers.size() == countGamersForRun) 
			{
				std::cout << "Runing GameGoom\n";
				Game::GroupToGame group{};
				for(int i = 0; i < countGamersForRun; ++i)
				{
					group.AddGamer(_waitingUsers.front());
					_waitingUsers.pop_front();
				}
				_roomsManager.CreateAndRunRoom(std::move(group));
			}
		}

		bool CancelRegistrationForGame(std::shared_ptr<UserConnection> user)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for(auto it = _waitingUsers.begin(); it != _waitingUsers.end(); ++it)
			{
				if(*it == user)
				{
					_waitingUsers.erase(it);
					return true;
				}
			}
			return false;
		}

	private:
		Game::RoomsManager _roomsManager;
		std::list<std::shared_ptr<UserConnection>> _waitingUsers;
		std::mutex _mutex;
	};
}

#endif