cmake_minimum_required(VERSION 2.8.4)
project(Email)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lssl")

set(SOURCE_FILES
    base64.cpp
    base64.h
    mailManager.cpp
    mailManager.h
    mailSocket.cpp
    mailSocket.h
    main.cpp)

add_executable(Email ${SOURCE_FILES})