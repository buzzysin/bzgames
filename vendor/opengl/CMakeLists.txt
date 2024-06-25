if(UNIX AND NOT APPLE)
  include(FindOpenGL)
  find_package(OpenGL)

  if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
  endif()

endif()