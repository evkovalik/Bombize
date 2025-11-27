#include <iostream>
#include <thread>

#include "Bmbz/Config/ServerConfig.hpp"
#include "Bmbz/Server/Server.hpp"
#include "Bmbz/Resources/ResourceLocator.hpp"

int main(int argc, char* argv[])
{
	Bmbz::Config::ServerConfig serverConfig{"D:\\GitHub\\BombizeServer\\jsonConfigs\\ServerConfig.json"};

	auto maps = std::make_unique<Bmbz::Resource::Maps>(serverConfig.GetGameMapsDirPath());
	maps->LoadAll();
	Bmbz::ResourceLocator::Provide(std::move(maps));

	auto characters = std::make_unique<Bmbz::Resource::Characters>();
	Bmbz::ResourceLocator::Provide(std::move(characters));

	boost::asio::io_context ioc;
	Bmbz::Server server{ioc, serverConfig};
	server.Run();
    
    auto&& t = std::thread([&ioc](){ioc.run();});
    std::cout << "Working..." << std::endl;
    t.join();

	return 0;
}