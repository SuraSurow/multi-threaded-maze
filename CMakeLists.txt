cmake_minimum_required(VERSION 3.29)
project(MultiThreadMaze)

# Ustawienie standardu C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Włącz pełne informacje o błędach podczas kompilacji (przydatne w przypadku debugowania)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# Dodanie plików źródłowych do projektu
add_executable(MultiThreadMaze main.cpp Maze.h Maze.cpp
        Generate.cpp)

