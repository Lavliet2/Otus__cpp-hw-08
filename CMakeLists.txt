cmake_minimum_required(VERSION 3.5)

project(homework_08 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(homework_08 main.cpp)


install(TARGETS homework_08
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


