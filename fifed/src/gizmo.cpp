#include "gizmo.hpp"
#include "editor.hpp"

namespace fifed {
	constexpr Color ORIGIN_COLOR = Color(20, 20, 255, 200);
	constexpr Color ORIGIN_COLOR_HOVERED = Colors::BLUE;

	constexpr Color X_AXIS_COLOR = Color(255, 20, 20, 200);
	constexpr Color X_AXIS_COLOR_HOVERED = Colors::RED;

	constexpr Color Y_AXIS_COLOR = Color(20, 255, 20, 200);
	constexpr Color Y_AXIS_COLOR_HOVERED = Colors::GREEN;

	constexpr std::array<vec2, 4> ORIGIN_UVS = {
		vec2(0.0f, 0.5f),
		vec2(0.0f, 1.0f),
		vec2(24.0f / 72.0f, 0.5f),
		vec2(24.0f / 72.0f, 1.0f),
	};

	constexpr std::array<vec2, 4> TRANSLATE_UVS = {
		vec2(24.0f / 72.0f, 0.0f),
		vec2(24.0f / 72.0f, 1.0f),
		vec2(48.0f / 72.0f, 1.0f),
		vec2(48.0f / 72.0f, 0.0f),
	};

	constexpr std::array<vec2, 4> SCALE_UVS = {
		vec2(48.f / 72.0f, 0.0f),
		vec2(48.f / 72.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f),
	};

	constexpr vec2 ARROW_SIZE = vec2(16 * 2.0f, 48 * 2.0f);

	Gizmo::Gizmo(Editor &editor) :
		m_Editor(editor),
		mp_Texture(std::make_shared<Texture>("assets/textures/gizmos.png", true, GL_NEAREST)),
		m_OriginPart({.offset = {0, 0}, .size = {16, 16}, .color = ORIGIN_COLOR, .hoverColor = ORIGIN_COLOR_HOVERED}),
		m_XAxisPart({.rotation = glm::radians(-90.0f), .offset = {ARROW_SIZE.y, 0}, .size = ARROW_SIZE, .color = X_AXIS_COLOR, .hoverColor = X_AXIS_COLOR_HOVERED}),
		m_YAxisPart({.rotation = glm::radians(180.0f), .offset = {0, ARROW_SIZE.y}, .size = ARROW_SIZE, .color = Y_AXIS_COLOR, .hoverColor = Y_AXIS_COLOR_HOVERED}) {}

	Gizmo::~Gizmo() { Logger::debug("F: %l", mp_Texture.use_count()); }

	void Gizmo::render() {
		if(!m_Editor.m_SelectedEntity) {
			return;
		}

		Renderer2D &renderer = GfxModule::get_instance().get_renderer2D();
		if(TransformComponent *trans = m_Editor.m_SelectedEntity.try_get_component<TransformComponent>()) {
			renderer.render_quad(trans->position, vec2(16, 16));
			const OrthoCamera &cam = renderer.get_camera();
			const vec2 cameraSize = cam.get_size();
			const f32 zoomFactor = std::min(cameraSize.x, cameraSize.y) / OrthoCamera::BASE_ZOOM;
			const vec2 mousePositionWorld = cam.screen_to_world(InputModule::get_instance().get_mouse_position());

			const vec2 scaledArrowSize = ARROW_SIZE * zoomFactor;
			const vec2 scaledOriginSize = m_OriginPart.size * zoomFactor;

			if(mousePositionWorld.x >= trans->position.x - scaledOriginSize.x && mousePositionWorld.x <= trans->position.x + scaledOriginSize.x &&
			   mousePositionWorld.y >= trans->position.y - scaledOriginSize.y && mousePositionWorld.y <= trans->position.y + scaledOriginSize.y) {
				mp_HoveredPart = &m_OriginPart;
			} else if(mousePositionWorld.x >= trans->position.x - scaledOriginSize.x && mousePositionWorld.x <= trans->position.x + scaledArrowSize.x && mousePositionWorld.y >= trans->position.y &&
					  mousePositionWorld.y <= trans->position.y + scaledArrowSize.y * 2) {
				mp_HoveredPart = &m_YAxisPart;
			} else if(mousePositionWorld.x >= trans->position.x && mousePositionWorld.x <= trans->position.x + scaledArrowSize.y * 2 && mousePositionWorld.y >= trans->position.y - scaledArrowSize.x &&
					  mousePositionWorld.y <= trans->position.y + scaledArrowSize.x) {
				mp_HoveredPart = &m_XAxisPart;
			} else {
				mp_HoveredPart = nullptr;
			}

			const auto render_gizmo_part = [&](GizmoPart &part, const std::array<vec2, 4> &uvs) {
				renderer.render_sprite(mp_Texture, trans->position + part.offset * zoomFactor, part.size * zoomFactor, part.rotation, &part == mp_HoveredPart ? part.hoverColor : part.color, uvs);
			};

			render_gizmo_part(m_XAxisPart, TRANSLATE_UVS);
			render_gizmo_part(m_YAxisPart, TRANSLATE_UVS);
			render_gizmo_part(m_OriginPart, ORIGIN_UVS);
		}
	}
}// namespace fifed
