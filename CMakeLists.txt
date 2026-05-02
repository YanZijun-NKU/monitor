cmake_minimum_required(VERSION 3.10)

project(monitor)

set(CMAKE_CXX_STANDARD 17)

find_package(wxWidgets REQUIRED COMPONENTS core base)

include(${wxWidgets_USE_FILE})

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(monitor ${SOURCES})

target_link_libraries(monitor ${wxWidgets_LIBRARIES})