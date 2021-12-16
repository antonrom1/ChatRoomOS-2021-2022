cmake_minimum_required(VERSION 3.20)
project(ProjetChat)

set(CMAKE_CXX_STANDARD 20)

add_executable(Server src/server/server.cpp src/common.h)
add_executable(Client src/client/client.cpp src/common.h)
