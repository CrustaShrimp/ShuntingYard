cmake_minimum_required(VERSION 4.0)
project(ShuntingYard)

set(CMAKE_CXX_STANDARD 23)

include_directories(ParseMath)

add_library(ShuntingYard
        ParseMath/CMathTokenOperator.h
        ParseMath/ShuntingYard.cpp
        ParseMath/ShuntingYard.h)

add_executable(ShuntingYardTest ParseMath/Main.cpp)
target_link_libraries(ShuntingYardTest ShuntingYard)
