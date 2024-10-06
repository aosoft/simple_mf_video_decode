cmake_minimum_required(VERSION 3.29)
project(simple_mf_video_decode)

set(CMAKE_CXX_STANDARD 23)

add_executable(simple_mf_video_decode WIN32
        main.cpp
        main_window.cpp)
