cmake_minimum_required(VERSION 3.8)
project(Mail)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp mailSocket.cpp mailSocket.h base64.cpp base64.h mailManager.cpp mailManager.h)
add_executable(Mail ${SOURCE_FILES})