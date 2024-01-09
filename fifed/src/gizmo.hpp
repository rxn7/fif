#pragma once

#include "ecs/components/transform_component.hpp"
namespace fifed {
	class Editor;

	enum class GizmoType : u8 { Translate, Scale, Rotate };

	struct GizmoPart {
		f32 rotation = 0.0f;
		vec2 offset;
		vec2 size;
		Color color;
		Color hoverColor;
	};

	class Gizmo {
	public:
		Gizmo(Editor &editor);
		~Gizmo();
		void render();

	private:
		void render_arrow(const Color &color, const Color &hoverColor, const vec2 &position);

	public:
		GizmoType m_Type;

	private:
		Editor &m_Editor;
		std::shared_ptr<Texture> mp_Texture;
		GizmoPart m_OriginPart;
		GizmoPart m_XAxisPart;
		GizmoPart m_YAxisPart;
		GizmoPart *mp_HoveredPart;
	};
}// namespace fifed
