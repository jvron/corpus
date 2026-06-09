add_library(engine STATIC)


target_include_directories(engine
    PUBLIC  include/   # Anyone linking 'engine' (the game) gets access to this folder
    PRIVATE src/       # ONLY 'engine' source files get access to this folder
)

# Source files
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

target_sources(engine PRIVATE ${ENGINE_SRC})

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
    PUBLIC glfw
    PUBLIC glad
)