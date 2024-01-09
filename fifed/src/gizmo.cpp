#include "gizmo.hpp"
#include "editor.hpp"
#include "panels/viewport/viewport_panel.hpp"

#include <fif/core/event/mouse_event.hpp>

namespace fifed {
	constexpr Color ORIGIN_COLOR = Color(20, 20, 255, 155);
	constexpr Color ORIGIN_COLOR_HOVERED = Colors::BLUE;

	constexpr Color X_AXIS_COLOR = Color(255, 20, 20, 155);
	constexpr Color X_AXIS_COLOR_HOVERED = Colors::RED;

	constexpr Color Y_AXIS_COLOR = Color(20, 255, 20, 155);
	constexpr Color Y_AXIS_COLOR_HOVERED = Colors::GREEN;

	constexpr std::array<vec2, 4> ORIGIN_UVS = {
		vec2(0.0f, 0.5f),
		vec2(0.0f, 1.0f),
		vec2(24.0f / 72.0f, 1.0f),
		vec2(24.0f / 72.0f, 0.5f),
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
		m_Camera(GfxModule::get_instance().get_renderer2D().get_camera()),
		mp_Texture(std::make_shared<Texture>("assets/textures/gizmos.png", true, GL_NEAREST)),
		m_OriginPart({.offset = {0, 0}, .size = {16, 16}, .color = ORIGIN_COLOR, .hoverColor = ORIGIN_COLOR_HOVERED}),
		m_XAxisPart({.rotation = glm::radians(-90.0f), .offset = {ARROW_SIZE.y * 0.5f, 0}, .size = ARROW_SIZE, .color = X_AXIS_COLOR, .hoverColor = X_AXIS_COLOR_HOVERED}),
		m_YAxisPart({.rotation = glm::radians(180.0f), .offset = {0, ARROW_SIZE.y * 0.5f}, .size = ARROW_SIZE, .color = Y_AXIS_COLOR, .hoverColor = Y_AXIS_COLOR_HOVERED}) {}

	Gizmo::~Gizmo() { Logger::debug("F: %l", mp_Texture.use_count()); }

	void Gizmo::render() {
		if(!m_Editor.m_SelectedEntity) {
			return;
		}

		Renderer2D &renderer = GfxModule::get_instance().get_renderer2D();
		if(TransformComponent *trans = m_Editor.m_SelectedEntity.try_get_component<TransformComponent>()) {
			const OrthoCamera &cam = renderer.get_camera();
			const vec2 cameraSize = cam.get_size();
			const f32 zoomFactor = std::min(cameraSize.x, cameraSize.y) / OrthoCamera::BASE_ZOOM;

			update_hover(zoomFactor, cam, *trans);

			const auto render_gizmo_part = [&](GizmoPart &part, const std::array<vec2, 4> &uvs) {
				renderer.render_sprite(mp_Texture, trans->position + part.offset * zoomFactor, part.size * zoomFactor, part.rotation, &part == mp_HoveredPart ? part.hoverColor : part.color, uvs);
			};

			switch(m_Mode) {
			case GizmoMode::Translate:
				render_gizmo_part(m_XAxisPart, TRANSLATE_UVS);
				render_gizmo_part(m_YAxisPart, TRANSLATE_UVS);
				render_gizmo_part(m_OriginPart, ORIGIN_UVS);
				break;

			case GizmoMode::Scale:
				render_gizmo_part(m_XAxisPart, SCALE_UVS);
				render_gizmo_part(m_YAxisPart, SCALE_UVS);
				render_gizmo_part(m_OriginPart, ORIGIN_UVS);
				break;

			default:
				break;
			}
		}
	}

	// TODO: Local/Global modes!

	void Gizmo::on_event(Event &event) {
		EventDispatcher::dispatch<MouseMovedEvent>(event, [&](MouseMovedEvent &movedEvent) {
			if(mp_ActivePart == nullptr)
				return false;

			InputModule &input = InputModule::get_instance();

			if(TransformComponent *trans = m_Editor.m_SelectedEntity.try_get_component<TransformComponent>()) {
				const vec2 mouseWorldPosition = m_Camera.screen_to_world(movedEvent.get_position());
				const vec2 lastMouseWorldPosition = m_Camera.screen_to_world(input.get_last_mouse_position());
				const vec2 delta = mouseWorldPosition - lastMouseWorldPosition;

				switch(m_Mode) {
				default:
				case GizmoMode::Translate:
					if(mp_ActivePart == &m_XAxisPart)
						trans->position.x += delta.x;
					else if(mp_ActivePart == &m_YAxisPart)
						trans->position.y += delta.y;
					else if(mp_ActivePart == &m_OriginPart)
						trans->position += delta;
					break;

				case GizmoMode::Scale:
					const vec2 value = delta * 0.01f;
					if(mp_ActivePart == &m_XAxisPart)
						trans->scale.x += value.x;
					else if(mp_ActivePart == &m_YAxisPart)
						trans->scale.y += value.y;
					else if(mp_ActivePart == &m_OriginPart)
						trans->scale += value.x + value.y;
					break;
				}
			}

			return true;
		});

		EventDispatcher::dispatch<MouseButtonPressedEvent>(event, [&](MouseButtonPressedEvent &event) {
			if(!m_Editor.get_viewport_panel().is_hovered())
				return false;

			if(!mp_HoveredPart || event.get_button() != GLFW_MOUSE_BUTTON_LEFT)
				return false;

			mp_ActivePart = mp_HoveredPart;
			return true;
		});

		EventDispatcher::dispatch<MouseButtonReleasedEvent>(event, [&](MouseButtonReleasedEvent &event) {
			if(!mp_ActivePart || event.get_button() != GLFW_MOUSE_BUTTON_LEFT)
				return false;

			mp_ActivePart = nullptr;
			return true;
		});
	}

	void Gizmo::update_hover(const f32 zoomFactor, const OrthoCamera &cam, const TransformComponent &trans) {
		const vec2 mousePositionWorld = cam.screen_to_world(InputModule::get_instance().get_mouse_position());
		const vec2 scaledHalfArrowSize = ARROW_SIZE * zoomFactor * 0.5f;
		const vec2 scaledHalfOriginSize = m_OriginPart.size * zoomFactor * 0.5f;

		if(mousePositionWorld.x >= trans.position.x - scaledHalfOriginSize.x && mousePositionWorld.x <= trans.position.x + scaledHalfOriginSize.x &&
		   mousePositionWorld.y >= trans.position.y - scaledHalfOriginSize.y && mousePositionWorld.y <= trans.position.y + scaledHalfOriginSize.y) {
			mp_HoveredPart = &m_OriginPart;
		} else if(mousePositionWorld.x >= trans.position.x - scaledHalfOriginSize.x && mousePositionWorld.x <= trans.position.x + scaledHalfArrowSize.x && mousePositionWorld.y >= trans.position.y &&
				  mousePositionWorld.y <= trans.position.y + scaledHalfArrowSize.y * 2) {
			mp_HoveredPart = &m_YAxisPart;
		} else if(mousePositionWorld.x >= trans.position.x && mousePositionWorld.x <= trans.position.x + scaledHalfArrowSize.y * 2 && mousePositionWorld.y >= trans.position.y - scaledHalfArrowSize.x &&
				  mousePositionWorld.y <= trans.position.y + scaledHalfArrowSize.x) {
			mp_HoveredPart = &m_XAxisPart;
		} else {
			mp_HoveredPart = nullptr;
		}

		ImGui::SetMouseCursor(mp_HoveredPart ? ImGuiMouseCursor_Hand : ImGuiMouseCursor_Arrow);
	}
}// namespace fifed
