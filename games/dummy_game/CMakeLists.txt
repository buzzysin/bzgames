bz_app(dummy_game
  PARENT
    games
  DEPENDS
    bz::core
    bz::engine::common
    bz::engine::engine
    bz::engine::graphics
    OpenGL
    GLEW
    glfw
  SOURCES
    src/main.cpp
    src/dummy_game.cpp
    src/dummy_scene.cpp
    src/dummy_renderer.cpp
  )

  # Clean up resources
add_custom_command(TARGET bz_dummy_game PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory
  $<TARGET_FILE_DIR:bz_dummy_game>/resources
  COMMENT "Cleaning up resources"
  )
  
  # Deploy resources to build directory (copy $CMAKE_SOURCE_DIR/resources to $CMAKE_BINARY_DIR/resources)
add_custom_command(TARGET bz_dummy_game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:bz_dummy_game>/resources
  COMMENT "Copying resources to build directory"
  )