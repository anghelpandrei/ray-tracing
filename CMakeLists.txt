cmake_minimum_required(VERSION 3.22)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracing main.cpp Object.cpp Wall.cpp Transmitter.cpp Receiver.cpp)

include_directories("${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL/include")
target_link_libraries(RayTracing "${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL/lib/x64/SDL2main.lib")
target_link_libraries(RayTracing "${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL/lib/x64/SDL2.lib")
target_link_libraries(RayTracing "${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL/lib/x64/SDL2_image.lib")
message(STATUS "${CMAKE_CURRENT_LIST_DIR}/dependencies/SDL/lib/x64/SDL2.lib")