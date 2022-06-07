cmake_minimum_required(VERSION 3.13)
project(ncurses_project)
set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES main.cpp)

add_executable(main ${SOURCE_FILES})
target_link_libraries(main ${CURSES_LIBRARIES})
