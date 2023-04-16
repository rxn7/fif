#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include "GLES3/gl3.h"
#else
#include "glad/glad.h"
#endif

#include "GLFW/glfw3.h"
