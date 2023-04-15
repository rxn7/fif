#include "editorLayer.h"
#include "fif/gfx/renderer2d.h"

#include <cmath>

static float x = 0.0f;

void EditorLayer::update(float dt) {
	x += dt;
}

void EditorLayer::render() {
	fif::gfx::Renderer2D::renderQuad({std::cos(x),std::sin(x)}, {1, 1}, {1.0f, 0.0f, 1.0f, 1.0f});
	fif::gfx::Renderer2D::renderCircle({std::sin(x),std::cos(x)}, 1, {1.0f, 1.0f, 0.0f, 1.0f});
}
