cmake_minimum_required(VERSION 3.20)
project(CardGame LANGUAGES C CXX)  # Enable both C and C++

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GLFW library
find_package(glfw3 3.3 REQUIRED)

# Find OpenGL library
find_package(OpenGL REQUIRED)

# Include directories (GLAD + any headers)
include_directories(
    include        # for glad/glad.h and KHR/khrplatform.h
)

# Add your main executable
add_executable(CardGame
    src/main.cpp
)

# Compile GLAD as a separate C library
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

# Link the libraries to your executable
target_link_libraries(CardGame PRIVATE glad glfw OpenGL::GL)
