cmake_minimum_required(VERSION 3.21)
project(HW3_3)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_INCLUDE_DIR SDL/include)
set(SDL2_LIB_DIR SDL/lib/x64)
#set(SDL2_INCLUDE_DIR C:/libs/SDL/include)
#set(SDL2_LIB_DIR C:/libs/SDL/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(HW3_3 main.cpp SnakeFun/Snake.cpp SnakeFun/Snake.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)