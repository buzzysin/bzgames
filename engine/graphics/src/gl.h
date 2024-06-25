// Ensures the proper include order for glew.h and the rest of the OpenGL
// headers.

#pragma once

// IWYU pragma: begin_exports

// IWYU pragma: no_include <GL/glew.h>
// IWYU pragma: no_include <GL/glu.h>
// IWYU pragma: no_include <GLFW/glfw3.h>

// First
#include <GL/glew.h>
// Then
#include <GL/glu.h>
#include <GLFW/glfw3.h>

// It's probably bad practice to include header that I don't necessarily need,
// but the sensitivity of the order of these includes is important.

// IWYU pragma: end_exports