cmake_minimum_required(VERSION 3.12)
project(Wet1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Wet1
        AvlTree.h
        Company.cpp
        Company.h
        compare.h
        HighTech.cpp
        HighTech.h
        library1.cpp
        library1.h
        main1.cpp
        rankAndId.h
        Worker.h)
