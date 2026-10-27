cmake_minimum_required(VERSION 3.20)

project(Minecraft-Multiplayer)

file(GLOB_RECURSE CONFIGURE_DEPENDS SOURCES src/*.cpp)

add_executable(Minecraft-Multiplayer
  src/main.cpp
  src/shader.cpp
  src/chunk.cpp
  src/world.cpp
  src/player.cpp
  src/renderer.cpp
  src/camera.cpp
  external/glad/src/glad.c
)

target_include_directories(Minecraft-Multiplayer
  PRIVATE
    include
    external/glad/include
)

target_compile_features(Minecraft-Multiplayer
  PRIVATE
    cxx_std_20
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(Minecraft-Multiplayer
  PRIVATE
  OpenGL::GL
    glfw
    glm
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


