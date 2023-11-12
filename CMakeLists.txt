cmake_minimum_required(VERSION 3.26)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glad)

add_executable(OpenGL src/main.cpp)
target_link_libraries(OpenGL glfw glad)
target_include_directories(OpenGL PUBLIC thirdparty/glad/include thirdparty/glfw/include)
