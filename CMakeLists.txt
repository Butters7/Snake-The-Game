cmake_minimum_required(VERSION 3.21)
project(Snake_The_Game)

set(CMAKE_CXX_STANDARD 20)

set(RESOURCES_FOLDER ${CMAKE_SOURCE_DIR}/Files)
set(SOURCES_FOLDER ${CMAKE_SOURCE_DIR}/src)
set(BINARY_FOLDER ${CMAKE_SOURCE_DIR}/bin)
set(RESOURCES_FILE ${CMAKE_SOURCE_DIR}/Files/gameres.res)
set(RC_FILE ${CMAKE_SOURCE_DIR}/Files/gameres.rc)
set(SOURCE_FILE main.cpp)

configure_file(${BINARY_FOLDER}/SDL2.dll ${CMAKE_SOURCE_DIR}/cmake-build-debug/SDL2.dll COPYONLY WIN32)
configure_file(${BINARY_FOLDER}/SDL2_mixer.dll ${CMAKE_SOURCE_DIR}/cmake-build-debug/SDL2_mixer.dll COPYONLY WIN32)

set(SDL2_INCLUDE_DIR SDL/include)
set(SDL2_LIB_DIR SDL/lib/x64)
set(SDL2_MIXER_INCLUDE_DIR SDL2_mixer/include)
set(SDL2_MIXER_LIB_DIR SDL2_mixer/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
link_directories(${SDL2_MIXER_LIB_DIR})

add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILE}
        ${SOURCES_FOLDER}/Consts/Consts.h
        ${SOURCES_FOLDER}/Block/Block.h
        ${SOURCES_FOLDER}/Fruit/Fruit.h ${SOURCES_FOLDER}/Fruit/BigFruit.h
        ${SOURCES_FOLDER}/Game/Game.h ${SOURCES_FOLDER}/Game/Game.cpp
        ${SOURCES_FOLDER}/Snake/Snake.h ${SOURCES_FOLDER}/Snake/Snake.cpp
        ${RC_FILE})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} SDL2main SDL2 SDL2_mixer)
target_sources(${PROJECT_NAME} PRIVATE ${RC_FILE})