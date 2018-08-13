cmake_minimum_required(VERSION 3.12)
project(DS_Wet1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(DS_Wet1
        AvlTree.h
        Company.cpp
        Company.h
        HighTech.cpp
        HighTech.h
        library1.cpp
        library1.h
        main1.cpp
        Worker.h compare.h)
