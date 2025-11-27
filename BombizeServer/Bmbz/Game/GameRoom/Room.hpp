#ifndef BMBZ_GAME_GAMEROOM_HPP
#define BMBZ_GAME_GAMEROOM_HPP

#include <memory>
#include <list>
#include "Participant.hpp"
#include "Generators/GenUniqueValue.hpp"
#include "Game/GameContext/GameContext.hpp"
#include "RewardCalculator.hpp"

namespace Bmbz
{
	class UserConnection;
	namespace Resource { struct MapInfo; }
namespace Game
{
	class RoomGroup;
	struct GroupToGame;
	
	class Room
	{
	public:
		Room(RoomGroup& group, uint16_t numberRoom);
		~Room();

		uint16_t Number() const { return _number; }
		void Run(
			GroupToGame&& gamers,
			std::function<void(int)> callbackFinish
		);
		void ProcessingInputData(
			boost::beast::flat_buffer& inBuffer,
			const boost::asio::ip::udp::endpoint& remoteEndpoint
		);

	private:
		RoomGroup& _group;
		uint16_t _number;
		std::function<void(int)> _callbackFinishGame;
		
		boost::asio::steady_timer _timerUpdateLoop;
		static constexpr std::chrono::milliseconds START_DELAY_TIME{3000};
		static constexpr std::chrono::milliseconds LOOP_TIME{100};
		
		std::chrono::time_point<std::chrono::steady_clock> _gameStartTime;
		std::chrono::time_point<std::chrono::steady_clock> _frameStartTime;
		std::chrono::time_point<std::chrono::steady_clock> _frameEndTime;

		Generator::GenUniqueValue _genAccessKeys;
		GameContext _gameContext;
		RewardCalculator _rewardCalculator;

		enum class GameState
		{
			Init,
			Starting,
			Running,
			Finish
		} _gameState = GameState::Init;

		struct ParticipantNode
		{
			uint32_t accessKey;
			Participant item;
			int winningPlace;
		};
		std::list<ParticipantNode> _participants;
		int _nextWinningPlace = 0;
		
		void Start();
		void Update();
		void GameLoopIteration();
		void FinishGame();
		void ErrorFinishGame();

		void SendGameActions();
		void OnClosingRoom(std::shared_ptr<UserConnection> user);
		void ReleaseParticipant(ParticipantNode& node);
		void EventDeathOfAvata(Participant& participant);
		
		void CreateWorld(Resource::MapInfo&& mapInfo);
	};
}
}
#endif