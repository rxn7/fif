#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/resource/font.hpp"
#include "fif/gfx/resource/texture.hpp"
#include "fif/gfx/text/text_align.hpp"

namespace fif::gfx {
	class Renderer2D;

	struct RenderCommand {
		virtual void render(Renderer2D &renderer) = 0;

		i8 zIndex = 0;
	};

	struct RenderCommandComparator final {
		bool operator()(const std::shared_ptr<RenderCommand> &a, const std::shared_ptr<RenderCommand> &b) const { return a->zIndex > b->zIndex; }
	};

	struct QuadRenderCommand final : public RenderCommand {
		QuadRenderCommand(const vec2 &_position, const vec2 &_size, const f32 _angle = 0.0f, const Color &_color = Colors::WHITE, const vec2 &_pivot = vec2(0.0f, 0.0f)) :
			position(_position), size(_size), angle(_angle), color(_color), pivot(_pivot) {}

		void render(Renderer2D &renderer) override;

		vec2 position = vec2(0.0f, 0.0f);
		vec2 size = vec2(100.0f, 100.0f);
		f32 angle = 0.0f;
		Color color = Colors::WHITE;
		vec2 pivot = vec2(0.0f, 0.0f);
	};

	struct CircleRenderCommand final : public RenderCommand {
		CircleRenderCommand(const vec2 &_position, const f32 _radius, const Color &_color = Colors::WHITE) : position(_position), radius(_radius), color(_color) {}

		void render(Renderer2D &renderer) override;

		vec2 position = vec2(0.0f, 0.0f);
		f32 radius = 50.0f;
		Color color = Colors::WHITE;
	};

	struct SpriteRenderCommand final : public RenderCommand {
		SpriteRenderCommand(const vec2 &_position, const vec2 &_size, const f32 _angle = 0.0f, const Color &_color = Colors::WHITE, const std::shared_ptr<Texture> &_p_Texture = nullptr, const vec2 &_pivot = vec2(0.0f, 0.0f), const u16vec2 &_uvPivot = u16vec2(0, 0), const u16vec2 &_uvSize = u16vec2(0, 0)) :
			position(_position), size(_size), angle(_angle), color(_color), p_Texture(_p_Texture), pivot(_pivot), uvPivot(_uvPivot), uvSize(_uvSize) {}

		void render(Renderer2D &renderer) override;

		vec2 position;
		vec2 size;
		f32 angle;
		Color color;
		std::shared_ptr<Texture> p_Texture;
		vec2 pivot;
		u16vec2 uvPivot;
		u16vec2 uvSize;// NOTE: Set to (0,0) to use full p_Texture size.
	};

	struct TextRenderCommand final : public RenderCommand {
		TextRenderCommand(const std::string &_text, const Font &_font, const vec2 &_position, const vec2 &_size, const f32 _angle, const Color &_color = Colors::BLACK, const VerticalTextAlign _vAlign = VerticalTextAlign::Center, const HorizontalTextAlign _hAlign = HorizontalTextAlign::Left) :
			text(_text), font(_font), position(_position), size(_size), angle(_angle), color(_color), vAlign(_vAlign), hAlign(_hAlign) {}

		void render(Renderer2D &renderer) override;

		std::string text;
		const Font &font;
		vec2 position;
		vec2 size;
		f32 angle;
		Color color = Colors::BLACK;
		VerticalTextAlign vAlign = VerticalTextAlign::Center;
		HorizontalTextAlign hAlign = HorizontalTextAlign::Left;
	};
}// namespace fif::gfx
