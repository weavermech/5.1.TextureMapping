cmake_minimum_required(VERSION 3.12)
project(5.1.TextureMapping)

include_directories(../common)


set(CMAKE_CXX_STANDARD 14)

add_executable(5.1.TextureMapping
        main.cpp
        ../common/Shader.cpp
        ../common/Vector.cpp
        ../common/Matrix.cpp
        ../common/Mesh.cpp
        ../common/SphericalCameraManipulator.cpp
        ../common/Texture.cpp)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLU REQUIRED)

target_link_libraries(5.1.TextureMapping GL ${GLUT_LIBRARY} GLEW GLU)