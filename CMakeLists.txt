cmake_minimum_required(VERSION 3.10)
project(Onshaper)

set(CMAKE_CXX_STANDARD 11)

add_executable(Onshaper main.cpp)

find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

target_link_libraries(Onshaper PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(Onshaper PRIVATE ${GLUT_LIBRARIES})
target_link_libraries(Onshaper PRIVATE -lopengl32 -g -lfreeglut)
target_link_libraries(Onshaper PRIVATE glm::glm)