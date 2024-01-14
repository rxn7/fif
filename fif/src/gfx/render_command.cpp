#include "fif/gfx/render_command.hpp"
#include "fif/gfx/renderer2d.hpp"

namespace fif::gfx {
	void QuadRenderCommand::render(Renderer2D &renderer) { renderer.render_quad(*this); }

	void CircleRenderCommand::render(Renderer2D &renderer) { renderer.render_circle(*this); }

	void SpriteRenderCommand::render(Renderer2D &renderer) { renderer.render_sprite(*this); }

	void TextRenderCommand::render(Renderer2D &renderer) { renderer.render_text(*this); }
}// namespace fif::gfx
