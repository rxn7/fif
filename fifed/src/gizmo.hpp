#pragma once

#include <fif/core/ecs/components/transform_component.hpp>

namespace fifed {
	class Editor;

	enum class GizmoMode : u8 { Translate, Scale, Rotate };
	enum class GizmoSpace : bool { Global, Local };

	struct GizmoPart {
		f32 rotation = 0.0f;
		vec2 size;
		Color color;
		Color hoverColor;
	};

	class Gizmo {
	public:
		Gizmo(Editor &editor);
		void render();
		void on_event(Event &event);

	private:
		void render_arrow(const Color &color, const Color &hoverColor, const vec2 &position);
		void update_hover(const f32 zoomFactor, const OrthoCamera &cam, const TransformComponent &trans);

	public:
		GizmoMode m_Mode = GizmoMode::Translate;
		GizmoSpace m_Space = GizmoSpace::Local;

	private:
		Editor &m_Editor;
		OrthoCamera &m_Camera;
		std::shared_ptr<Texture> mp_Texture;

		GizmoPart m_OriginPart;
		GizmoPart m_XAxisPart;
		GizmoPart m_YAxisPart;

		GizmoPart *mp_HoveredPart = nullptr;
		GizmoPart *mp_ActivePart = nullptr;
	};
}// namespace fifed
