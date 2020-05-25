cmake_minimum_required(VERSION 3.13)
project(mpi-insertion-srt)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER "mpic++")

include_directories(/usr/local/Cellar/open-mpi/4.0.1_2/include)

add_executable(mpi-insertion-srt ../mpi-insertion-srt/main.cpp)