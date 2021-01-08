cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME Hiya_Why_You_Use_Reinforcement_Learning_To_Play_Tetris_You_Dont_Know_How_To_Play_It_Yourself_Uncle_Roger_So_Sad_Now)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        src/main.cpp
        src/menu.cpp
        src/game.cpp
        src/window.cpp
        src/tetriscontroller.cpp
        src/tetromino.cpp
        src/animations.cpp
        src/server.cpp
        src/client.cpp)

include_directories(include)

include_directories(allegro/include)
link_directories(allegro/lib)

set(CMAKE_CXX_COMPILER /usr/bin/clang++ CACHE PATH "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries( ${PROJECT_NAME}
        allegro_acodec
        allegro_audio
        allegro_color
        allegro_dialog
        allegro_font
        allegro_image
        allegro_main
        allegro_memfile
        allegro_physfs
        allegro_primitives
        allegro
        allegro_ttf
        allegro_video
)