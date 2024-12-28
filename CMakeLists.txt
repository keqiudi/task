cmake_minimum_required(VERSION 3.16)
project(src C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(src
    main.c
    main.h
    main_core.c
    main_core.h
    socket.c
    socket.h
    thread.c
    thread.h)
