cmake_minimum_required(VERSION 3.10)
project(ShabeFramework)

set(CMAKE_CXX_STANDARD 11)
include_directories(src)
include_directories(libs)
set(SOURCE_FILES src/HttpStructures.c src/server.c libs/map.c src/Parser.c)
add_executable(shabe ${SOURCE_FILES} main.c)
