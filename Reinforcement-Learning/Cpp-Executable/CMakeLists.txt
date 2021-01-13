cmake_minimum_required(VERSION 3.17)
project(Cpp_Executable)

find_library(TENSORFLOW_LIB tensorflow HINT $ENV{HOME}/libtensorflow/lib)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(CMAKE_CXX_STANDARD 17)
add_executable(Cpp_Executable main.cpp src/Model.cpp src/Tensor.cpp src/client.cpp)
target_include_directories(Cpp_Executable PRIVATE include $ENV{HOME}/libtensorflow/include)
target_link_libraries (Cpp_Executable "${TENSORFLOW_LIB}")
