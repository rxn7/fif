#include "fif/gfx/gfx.h"
#include "fif/gfx/shaderLibrary.h"

#include "glad/glad.h"

void fif::gfx::init() {
	ShaderLibrary::init();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
