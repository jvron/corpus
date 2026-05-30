add_executable(game)

target_sources(game PRIVATE
    main.cpp
    test_input.cpp
)

target_link_libraries(game PRIVATE engine)