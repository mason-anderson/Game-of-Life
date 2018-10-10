cmake_minimum_required(VERSION 3.5)
project(gameOfLife)

set(CMAKE_CXX_STANDARD 14)
set(source_dir "{$PROJECT_SOURCE_DIR}/src/")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
set(executable_name "gameOfLife")

#set(SFML_STATIC_LIBRARIES TRUE)

set(source_files src/main.cpp src/Board.cpp src/Board.h)

include_directories(${source_dir})

find_package(SFML 2 REQUIRED network audio graphics window system)

add_executable(gameOfLife ${source_files})

target_link_libraries(${executable_name} sfml-graphics sfml-network sfml-window sfml-system sfml-audio)

