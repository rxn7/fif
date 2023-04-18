#pragma once

#ifdef __EMSCRIPTEN__
#include "GLES3/gl3.h"
#include <emscripten.h>
#else
#include "glad/glad.h"
#endif

#include "GLFW/glfw3.h"
