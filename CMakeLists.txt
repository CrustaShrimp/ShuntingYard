cmake_minimum_required(VERSION 4.0)
project(ShuntingYard)

set(CMAKE_CXX_STANDARD 20)

include_directories(ParseMath)

add_executable(ShuntingYard
        ParseMath/CMathTokenOperator.h
        ParseMath/ShuntingYard.cpp
        ParseMath/Main.cpp
        ParseMath/ShuntingYard.h)
