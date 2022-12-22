cmake_minimum_required(VERSION 3.21)
project(HW3_3)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
set(SOURCES_FOLDER ${CMAKE_SOURCE_DIR}/src)
set(RESOURCES_FILE ${CMAKE_SOURCE_DIR}/Files/gameres.res)
set(RC_FILE ${CMAKE_SOURCE_DIR}/Files/gameres.rc)


set(SDL2_INCLUDE_DIR SDL/include)
set(SDL2_LIB_DIR SDL/lib/x64)
set(SDL2_LIB ${CMAKE_SOURCE_DIR}/SDL/lib/x64/SDL2.lib)
set(SDL2_LIBMAIN ${CMAKE_SOURCE_DIR}/SDL/lib/x64/SDL2main.lib)
set(SDL2_LIBMIXER ${CMAKE_SOURCE_DIR}/SDL2_mixer/lib/x64/SDL2_mixer.lib)
set(SDL2_MIXER_INCLUDE_DIR SDL2_mixer/include)
set(SDL2_MIXER_LIB_DIR SDL2_mixer/lib/x64)

link_directories(${SDL2_LIB_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
link_directories(${SDL2_MIXER_LIB_DIR})
include_directories(${PROJECT_NAME} "${SOURCES_FOLDER}/Game ${SOURCES_FOLDER}/Snake")

add_executable(${PROJECT_NAME} WIN32 main.cpp
        ${SOURCES_FOLDER}/Consts/Consts.h
        ${SOURCES_FOLDER}/Block/Block.h
        ${SOURCES_FOLDER}/Fruit/Fruit.h
        ${SOURCES_FOLDER}/Game/Game.h ${SOURCES_FOLDER}/Game/Game.cpp
        ${SOURCES_FOLDER}/Snake/Snake.h ${SOURCES_FOLDER}/Snake/Snake.cpp
        ${RC_FILE} ${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_MIXER_LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIB} ${SDL2_LIBMAIN} ${SDL2_LIBMIXER})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_MIXER_LIB_DIR})
target_sources(${PROJECT_NAME} PRIVATE ${RC_FILE} ${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_MIXER_LIB_DIR})