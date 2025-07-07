cmake_minimum_required(VERSION 3.10)
project(Socket)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Server Server.cpp Mysocket.cpp)
add_executable(Client Client.cpp Mysocket.cpp)