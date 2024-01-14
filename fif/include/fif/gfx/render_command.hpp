#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/resource/font.hpp"
#include "fif/gfx/resource/texture.hpp"
#include "fif/gfx/text/text_align.hpp"

namespace fif::gfx {
	struct RenderCommand {
		i8 zIndex = 0;
	};

	struct RenderCommandComparator final {
		bool operator()(const std::unique_ptr<RenderCommand> &a, const std::unique_ptr<RenderCommand> &b) const { return a->zIndex < b->zIndex; }
	};

	struct QuadRenderCommand final : public RenderCommand {
		vec2 position = vec2(0.0f, 0.0f);
		vec2 size = vec2(100.0f, 100.0f);
		f32 angle = 0.0f;
		Color color = Colors::WHITE;
		vec2 pivot = vec2(0.0f, 0.0f);
	};

	struct CircleRenderCommand final : public RenderCommand {
		vec2 position = vec2(0.0f, 0.0f);
		f32 radius = 50.0f;
		Color color = Colors::WHITE;
	};

	struct SpriteRenderCommand final : public RenderCommand {
		vec2 position = vec2(0.0f, 0.0f);
		vec2 size = vec2(100.0f, 100.0f);
		f32 angle = 0.0f;
		Color color = Colors::WHITE;
		vec2 pivot = vec2(0.0f, 0.0f);

		std::shared_ptr<Texture> p_Texture;

		u16vec2 uvPivot{0u, 0u};

		// NOTE: Set to (0,0) to use full p_Texture size.
		u16vec2 uvSize{0u, 0u};
	};

	struct TextRenderCommand final : public RenderCommand {
		std::string text;
		const Font &font;
		vec2 position;
		vec2 size;
		Color color = Colors::BLACK;
		VerticalTextAlign vAlign = VerticalTextAlign::Center;
		HorizontalTextAlign hAlign = HorizontalTextAlign::Left;
	};
}// namespace fif::gfx
