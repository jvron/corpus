add_executable(sandbox)

target_sources(sandbox PRIVATE
    src/main.cpp
    src/mesh.cpp
    src/test.cpp
    src/material_loader.cpp
)

target_include_directories(sandbox PRIVATE
    include/
)

target_link_libraries(sandbox PRIVATE
    engine
)