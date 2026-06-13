add_library(engine STATIC)

# ------------------------
# Source files
# ------------------------
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

target_sources(engine PRIVATE ${ENGINE_SRC})

# ------------------------
# Engine include directories
# ------------------------
target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
)

# ------------------------
# GLAD
# ------------------------
add_library(glad STATIC
    external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

# ------------------------
# Dependencies
# ------------------------
target_link_libraries(engine
    PUBLIC
        glfw
        glad
)

# ------------------------
# Compiler warnings
# ------------------------
target_compile_options(engine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# ------------------------
# Optional
# ------------------------
# target_compile_options(engine PRIVATE -Werror)

# set_target_properties(engine PROPERTIES
#     CXX_CLANG_TIDY
#     "clang-tidy;-checks=modernize-*,bugprone-*,performance-*"
# )