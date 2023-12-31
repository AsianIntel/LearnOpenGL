cmake_minimum_required(VERSION 3.26)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/assimp)

add_executable(OpenGL src/main.cpp
        src/Shader.cpp
        src/Shader.h
        src/Camera.cpp
        src/Camera.h
        src/Mesh.cpp
        src/Mesh.h
        src/Model.cpp
        src/Model.h)
target_link_libraries(OpenGL glfw glad glm assimp)
target_include_directories(
        OpenGL PUBLIC
        thirdparty/glad/include
        thirdparty/glfw/include
        thirdparty/stb_image
        thirdparty/glm
        thirdparty/assimp/include
)
