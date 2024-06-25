if(UNIX AND NOT APPLE)
  include(FindGLEW)
  find_package(GLEW)

  if(NOT GLEW_FOUND)
    message(FATAL_ERROR "GLEW not found")
  endif()
endif()