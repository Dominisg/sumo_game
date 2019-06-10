cmake_minimum_required(VERSION 3.1)
project(sumo_pr)

set(CMAKE_CXX_STANDARD 14)

add_executable(sumo_pr client/main.cpp client/Sumo.cpp client/Sumo.h client/Game.cpp client/Menu.cpp client/Menu.h
        client/Game.h utils/Progressbar.cpp utils/Progressbar.h utils/EllipseShape.cpp utils/EllipseShape.h client/Ring.cpp
        client/Ring.h client/SocketHandler.cpp client/SocketHandler.h common/messages.h)

add_executable(sumo_server server/main.cpp server/Server.cpp server/Server.h common/messages.h server/BattleArea.cpp server/BattleArea.h utils/EllipseShape.cpp utils/EllipseShape.h)

target_link_libraries (sumo_pr sfml-graphics sfml-window sfml-system sfml-network)
target_link_libraries (sumo_server sfml-system sfml-graphics sfml-network)
