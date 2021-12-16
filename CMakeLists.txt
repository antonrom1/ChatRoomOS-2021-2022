cmake_minimum_required(VERSION 3.20)
project(ProjetChat)

set(CMAKE_CXX_STANDARD 20)

add_executable(Server src/server/server.cpp)
add_executable(Client src/client/client.cpp)
