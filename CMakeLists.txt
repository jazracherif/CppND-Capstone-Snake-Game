cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall -pthread")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include("${CMAKE_SOURCE_DIR}/cmake/rapidjson.cmake")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include src)

add_executable(SnakeGame src/main.cpp src/game.cpp src/controller.cpp src/renderer.cpp src/snake.cpp src/logger.cpp)
add_dependencies(SnakeGame rapidjson)

string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(SnakeGame ${SDL2_LIBRARIES})
