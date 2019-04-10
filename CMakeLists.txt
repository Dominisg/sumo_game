cmake_minimum_required(VERSION 3.1)
project(sumo_pr)

set(CMAKE_CXX_STANDARD 14)

add_executable(sumo_pr main.cpp Sumo.cpp Sumo.h Game.cpp Game.h utils/Progressbar.cpp utils/Progressbar.h)

target_link_libraries (sumo_pr sfml-graphics sfml-window sfml-system)
