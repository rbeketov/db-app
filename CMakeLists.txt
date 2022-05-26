cmake_minimum_required(VERSION 3.17)
project(practice)

SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++19 -Wall -Werror")

include_directories(project/include)
add_executable(practice 
                project/src/main.cpp 
                project/src/date.cpp 
                project/src/dbvalue.cpp 
                project/src/tabletxt.cpp
                project/src/service.cpp)