cmake_minimum_required(VERSION 3.16)
project(ProjetChat)

find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)

add_executable(Server src/server/server.cpp src/server/client.cpp src/server/chat_room.cpp src/common/error_handling.cpp src/server/clients_set.cpp src/server/clients_set.h src/server/username.cpp src/server/username.h)
add_executable(Client src/client/client.cpp src/common/error_handling.cpp)

target_link_libraries(Client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Client ${CURSES_LIBRARY})
