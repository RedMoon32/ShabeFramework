cmake_minimum_required(VERSION 3.10)
project(ShabeFramework)

set(CMAKE_CXX_STANDARD 11)
include_directories(src)
include_directories(libs)
include_directories(tests)
find_package(Threads)
set(SOURCE_FILES src/http_structures.c src/server.c libs/map.c src/parser.c src/dispatcher.c libs/alist.c)
add_executable(shabe ${SOURCE_FILES}  main.c)
add_executable(shabe_test ${SOURCE_FILES} tests/test_system.c)
target_link_libraries (shabe ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (shabe_test ${CMAKE_THREAD_LIBS_INIT})

