cmake_minimum_required(VERSION 3.21)
project(HW3_3)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_INCLUDE_DIR SDL/include)
set(SDL2_LIB_DIR SDL/lib/x64)
set(SDL2_MIXER_INCLUDE_DIR SDL2_mixer/include)
set(SDL2_MIXER_LIB_DIR SDL2_mixer/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
link_directories(${SDL2_MIXER_LIB_DIR})

add_executable(HW3_3 main.cpp src/Game/Game.h src/Game/Game.cpp src/Game/Snake.h src/Game/Snake.cpp)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
target_link_libraries(${PROJECT_NAME} SDL2_mixer)