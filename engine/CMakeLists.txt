add_library(engine STATIC)

# Source files

file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

target_sources(engine PRIVATE ${ENGINE_SRC})

# Engine include directories

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb
        
)

# glad

add_library(glad STATIC
    third_party/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
)

target_precompile_headers(engine PRIVATE
    <vector>
    <cstdint>
    <memory>
)


target_link_libraries(engine
    PUBLIC
        glfw
        glad
        assimp
)

# compile options

target_compile_options(engine PRIVATE -fdiagnostics-color=always)

# Compiler warnings

target_compile_options(engine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# set_target_properties(engine PROPERTIES
#     CXX_CLANG_TIDY
#     "clang-tidy;-checks=modernize-*,bugprone-*,performance-*"
# )