cmake_minimum_required(VERSION 3.22)
project(learnOpenGL)

set(CMAKE_CXX_STANDARD 14)

find_package (OpenGL REQUIRED)
find_package (GLUT REQUIRED)
include_directories(src)

add_executable(${PROJECT_NAME} main.cpp)
set(OpenGL_GL_PREFERENCE GLVND)

#add_subdirectory(glsdk)
#add_subdirectory(framework)
#
#function(add_tutorial base_dir tgt_name)
#    set(sources ${ARGN})
#    list(TRANSFORM sources PREPEND "${base_dir}/")
#    add_executable(${tgt_name} ${sources})
#    target_link_libraries(${tgt_name} PRIVATE framework)
#endfunction()

### copy all data files to the build directory
#file(COPY "../../Desktop/gltut/data" DESTINATION "${CMAKE_BINARY_DIR}")
#file(GLOB data_dirs "Tut*/data")
#foreach(data_dir ${data_dirs})
#    file(COPY "${data_dir}" DESTINATION "${CMAKE_BINARY_DIR}")
#endforeach()


#add_subdirectory(src)
#add_subdirectory(tst)
#add_subdirectory(lib/googletest)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${OPENGL_gl_LIBRARY} ${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY})
target_link_libraries(${PROJECT_NAME} libfreeglut.a libfreeglut_static.a ${OPENGL_gl_LIBRARY} -lOpenGL32 -lfreeGLUT -lglew32)