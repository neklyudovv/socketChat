cmake_minimum_required(VERSION 3.29)
project(socketChat)

set(CMAKE_CXX_STANDARD 20)

add_executable(server server.cpp)

add_executable(client client.cpp)
