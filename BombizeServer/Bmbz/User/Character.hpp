#ifndef BMBZ_USER_CHARACTERINFO_HPP
#define BMBZ_USER_CHARACTERINFO_HPP

#include <chrono>
#include <optional>

namespace Bmbz
{
namespace User
{
	struct Character
	{
		Character(uint32_t id, int64_t disappearanceTime)
		: id(id)
		, disappearanceTime(disappearanceTime > 0 ? 
			std::make_optional(std::chrono::system_clock::from_time_t(disappearanceTime)) :
			std::nullopt)
		{}

		Character(
			uint32_t id,
			std::optional<std::chrono::time_point<std::chrono::system_clock>> disappearanceTime
		)
		: id(id)
		, disappearanceTime(disappearanceTime)
		{}

		uint32_t id;
		std::optional<std::chrono::time_point<std::chrono::system_clock>> disappearanceTime;

		int64_t GetDisappearanceTimeAsInt64() const
		{
			if(disappearanceTime.has_value())
				return disappearanceTime.value().time_since_epoch().count();
			return 0;
		}
	};
}
}
#endif