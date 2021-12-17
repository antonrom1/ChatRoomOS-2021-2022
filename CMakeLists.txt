cmake_minimum_required(VERSION 3.20)
project(ProjetChat)

find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 20)

add_executable(Server src/server/server.cpp src/server/client.cpp src/server/chat_room.cpp src/common/error_handling.cpp src/server/clients_set.cpp src/server/clients_set.h src/server/username.cpp src/server/username.h)
add_executable(Client src/client/client.cpp src/common/error_handling.cpp)

target_link_libraries(Client ${CMAKE_THREAD_LIBS_INIT})
