cmake_minimum_required(VERSION 3.17)
project(Hiya_Server)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        src/main.cpp
        src/server.cpp)

set(CMAKE_CXX_COMPILER /usr/bin/clang++ CACHE PATH "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(include)

add_executable(Hiya_Server ${SOURCES})
