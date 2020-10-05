cmake_minimum_required(VERSION 3.12)
project(LoopLine)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5.1 COMPONENTS network audio graphics window system REQUIRED)

add_executable(LD47
    main.cpp 
    textureManager.cpp
    inputManager.cpp
    drawable.cpp
    train.cpp
    trainWagon.cpp
    rails.cpp
    game.cpp
    station.cpp
    passenger.cpp
    obstacle.cpp
    helper.cpp
    button.cpp
)

target_link_libraries(LD47 sfml-window sfml-graphics sfml-audio sfml-network)