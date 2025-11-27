cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
project(bombize_server VERSION 0.1.0 LANGUAGES C CXX)
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif()
set(Boost_NO_WARN_NEW_VERSIONS ON)

if(WIN32)
	message("Connecting the library for Windows platforn")
	set(BOOST_ROOT "D:/myLibs/boost_1_89_0")
	set(BOOST_LIBRARYDIR "${BOOST_ROOT}/bin/x64/lib")

	set(Boost_USE_STATIC_LIBS ON)
	set(Boost_USE_MULTITHREADED ON)
	# set(Boost_ARCHITECTURE "-x64")
endif()
find_package(Boost 1.89.0 REQUIRED COMPONENTS json)

set(SOURCES_GAME
	server.cpp
	Bmbz/Server/Server.cpp
	Bmbz/Server/UserConnection.cpp
	Bmbz/Database/GameDB.cpp
	Bmbz/Network/TcpListener.cpp
	Bmbz/Network/UdpMaster.cpp
	Bmbz/Network/WebSocketConnection.cpp
	Bmbz/Network/UdpPacketDeliverer.cpp
	Bmbz/Game/GameRoom/Room.cpp
	Bmbz/Game/GameRoom/InputController/InputController.cpp
	Bmbz/Game/GameRoom/Participant.cpp
	Bmbz/Game/GameObjects/Avatar.cpp
	Bmbz/Game/GameObjects/Map.cpp
	Bmbz/Prefabs/Map.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES_GAME})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Bmbz)

# <------- Boost -------> #

if(Boost_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${BOOST_ROOT})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
	message(STATUS "Found Boost ${Boost_VERSION_STRING}")
	message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")
else()
	message(STATUS "Boost not found")
endif()

# <------- MySQL Connector -------> #

if(UNIX)
	message("Connecting the MySQL Connector/C++ 9.3.0 Community library for Linux platforn")
	find_package(mysql-concpp REQUIRED PATHS "${CMAKE_SOURCE_DIR}/External/mysql_connector")
elseif(WIN32)
	message("Connecting the MySQL Connector/C++ 9.3.0 Community library for Windows platforn")
	find_package(mysql-concpp REQUIRED PATHS "${CMAKE_SOURCE_DIR}/External/mysql-connector-9_4_0-winx64-debug")
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE mysql::concpp)

# <------- END LINK -------> #

# BUILD_TESTS
option(BMBZ_TESTS "Build the testing tree" OFF)
if(BMBZ_TESTS)
	add_subdirectory(Testing)
endif()
message(STATUS "BUILD_TESTS is ${BMBZ_TESTS}")
