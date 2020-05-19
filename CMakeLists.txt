cmake_minimum_required(VERSION 3.16)
project(AnNum)

set(CMAKE_SOURCE_DIR .)

add_subdirectory(src)

include_directories(include)
include_directories(include/base)

#add_library(anarionbase STATIC IMPORTED)
#set_target_properties(anarionbase PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libanarionbase.a)

add_executable(AnNum main.cpp)
target_link_libraries(AnNum anarionnum)
