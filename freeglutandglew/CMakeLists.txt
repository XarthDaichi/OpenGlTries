cmake_minimum_required(VERSION 3.22)
project(freeglutandglew)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(freeglutandglew main.cpp)

include_directories(include/GL)
add_library(FREEGLUT freeglut_staticd lib)
#add_library(GLEW glew32 lib)
#target_link_libraries(freeglutandglew LINK_PUBLIC ${FREEGLUT} ${GLEW})