cmake_minimum_required(VERSION 3.24)
project(C_Web_Server C)

set(CMAKE_C_STANDARD 11)

include_directories(src)
include_directories(src/cache_tests)

add_executable(C_Web_Server
        src/cache_tests/cache_tests.c
        src/cache_tests/dbg.h
        src/cache_tests/minunit.h
        src/cache_tests/utils.h
        src/cache.c
        src/cache.h
        src/file.c
        src/file.h
        src/hashtable.c
        src/hashtable.h
        src/llist.c
        src/llist.h
        src/mime.c
        src/mime.h
        src/net.c
        src/net.h
        src/server.c)
