add_library(engine STATIC)

include(FetchContent)

# Glfw

set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)

FetchContent_MakeAvailable(glfw)

# Glad

add_library(glad STATIC
    third_party/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
)

# Assimp

find_package(assimp CONFIG REQUIRED)

# Engine include directories

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb       
)

# Source

file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

target_sources(engine PRIVATE ${ENGINE_SRC})

target_link_libraries(engine
    PUBLIC
        glfw
        glad
    PRIVATE
        assimp::assimp
)

# Compiler options
target_compile_options(engine PRIVATE
    -fdiagnostics-color=always
    -Wall
    -Wextra
    -Wpedantic
)