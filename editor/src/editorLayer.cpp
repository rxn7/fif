#include "editorLayer.h"
#include "fif/gfx/renderer2d.h"

void EditorLayer::render() {
	fif::gfx::Renderer2D::renderCircle({0,0}, 10);
}
