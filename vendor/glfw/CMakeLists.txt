# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# Bug: EGL is not supported on some platforms
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/build/_deps/glfw-3.4 
                 ${CMAKE_BINARY_DIR}/_deps/glfw-3.4)

message(STATUS "GLFW SOURCE DIR: ${CMAKE_SOURCE_DIR}/build/_deps/glfw-3.4")
message(STATUS "GLFW BINARY DIR: ${CMAKE_BINARY_DIR}/_deps/glfw-3.4")