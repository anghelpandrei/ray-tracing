cmake_minimum_required(VERSION 3.22)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracing main.cpp Object.cpp Wall.cpp Transmitter.cpp Receiver.cpp)
