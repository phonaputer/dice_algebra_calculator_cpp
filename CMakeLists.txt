cmake_minimum_required(VERSION 3.10.0)
project(dice_algebra_calculator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

add_executable(dice_algebra_calculator ${SOURCES})

