# If on Linux, link to the SDL2 and SDL2_image libraries
if(UNIX) # Does this work on MacOS? A: Yes, it does! This is because MacOS is a Unix-based system
  find_package(SDL2)

  if (NOT SDL2_FOUND)
    # Run bash -c ./setup.sh and try again
    message(FATAL_ERROR "SDL2 not found, please run ${CMAKE_CURRENT_SOURCE_DIR}/setup.sh")
  endif()

  include_directories(${SDL2_INCLUDE_DIRS})
elseif(WIN32)

endif()