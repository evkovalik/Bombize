cmake_minimum_required(VERSION 3.10)
project(ServerTests LANGUAGES CXX)

SET(TESTING_PROJECT TestMapDestroyer)
add_executable(${TESTING_PROJECT} TestMapDestroyer.cpp)
target_include_directories(${TESTING_PROJECT} PRIVATE ../Bmbz)
target_include_directories(${TESTING_PROJECT} PUBLIC ${BOOST_ROOT})
