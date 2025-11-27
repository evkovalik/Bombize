#ifndef BMBZ_USER_LOGINDATA_HPP
#define BMBZ_USER_LOGINDATA_HPP

#include "NetworkTypes/NetworkString.hpp"

namespace Bmbz
{
namespace User
{
	struct LoginData
	{
		NetType::NetworkString login;
		NetType::NetworkString password;

		LoginData() = default;

		LoginData(std::string login, std::string password)
		: login(std::move(login))
		, password(std::move(password))
		{}

		bool IsValid() const
		{
			return true;	// add check
		}

		void Deserialize(boost::beast::flat_buffer& buffer)
		{
			login.Deserialize(buffer);
			password.Deserialize(buffer);
		}

		void Serialize(boost::beast::flat_buffer& buffer) const
		{
			login.Serialize(buffer);
			password.Serialize(buffer);
		}
	};
}
}
#endif