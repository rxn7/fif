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

	constexpr u16vec2 ORIGIN_UV_PIVOT{0, 48};
	constexpr u16vec2 ORIGIN_UV_SIZE{24, 24};

	constexpr u16vec2 TRANSLATE_UV_PIVOT{24, 0};
	constexpr u16vec2 SCALE_UV_PIVOT{48, 0};

	constexpr u16vec2 ARROW_UV_SIZE{24, 48};

	constexpr vec2 ARROW_SIZE = vec2(16 * 4.f, 48 * 4.f);
	constexpr vec2 ARROW_PIVOT = vec2(0.0f, -ARROW_SIZE.y * 0.55f);

	Gizmo::Gizmo(Editor &editor) :
		m_Editor(editor), m_Camera(GfxModule::get_instance().get_renderer2D().get_camera()), mp_Texture(std::make_shared<Texture>("assets/textures/gizmos.png", true, GL_NEAREST)), m_OriginPart({.size = {24 * 2, 24 * 2}, .color = ORIGIN_COLOR, .hoverColor = ORIGIN_COLOR_HOVERED}), m_XAxisPart({.rotation = glm::radians(-90.0f), .size = ARROW_SIZE, .color = X_AXIS_COLOR, .hoverColor = X_AXIS_COLOR_HOVERED}), m_YAxisPart({.rotation = glm::radians(180.0f), .size = ARROW_SIZE, .color = Y_AXIS_COLOR, .hoverColor = Y_AXIS_COLOR_HOVERED}) {}

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

			if(mp_ActivePart) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			} else if(mp_HoveredPart) {
				if(mp_HoveredPart == &m_XAxisPart) {
					ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
				} else if(mp_HoveredPart == &m_YAxisPart) {
					ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
				} else {
					ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
				}
			}
			const auto render_gizmo_part = [&](GizmoPart &part, const u16vec2 &uvPivot, const u16vec2 &uvSize, const vec2 &pivot = vec2(0.0f)) {
				const f32 angleRadians = m_Space == GizmoSpace::Local ? part.rotation + trans->angleRadians : part.rotation;
				renderer.add_render_command<SpriteRenderCommand>(127, Renderer2D::INVALID_ENTITY_ID, trans->position, part.size * zoomFactor, angleRadians, (&part == mp_HoveredPart || &part == mp_ActivePart) ? part.hoverColor : part.color, mp_Texture, pivot * zoomFactor, uvPivot, uvSize);
			};

			switch(m_Mode) {
			case GizmoMode::Translate:
				render_gizmo_part(m_XAxisPart, TRANSLATE_UV_PIVOT, ARROW_UV_SIZE, ARROW_PIVOT);
				render_gizmo_part(m_YAxisPart, TRANSLATE_UV_PIVOT, ARROW_UV_SIZE, ARROW_PIVOT);
				render_gizmo_part(m_OriginPart, ORIGIN_UV_PIVOT, ORIGIN_UV_SIZE);
				break;

			case GizmoMode::Scale:
				render_gizmo_part(m_XAxisPart, SCALE_UV_PIVOT, ARROW_UV_SIZE, ARROW_PIVOT);
				render_gizmo_part(m_YAxisPart, SCALE_UV_PIVOT, ARROW_UV_SIZE, ARROW_PIVOT);
				render_gizmo_part(m_OriginPart, ORIGIN_UV_PIVOT, ORIGIN_UV_SIZE);
				break;

			default:
				break;
			}
		}
	}

	void Gizmo::on_event(Event &event) {
		InputModule &input = InputModule::get_instance();

		EventDispatcher::dispatch<MouseMovedEvent>(event, [&](MouseMovedEvent &movedEvent) {
			if(mp_ActivePart == nullptr)
				return false;

			if(TransformComponent *trans = m_Editor.m_SelectedEntity.try_get_component<TransformComponent>()) {
				const vec2 lastGlobalMousePosition = m_Camera.screen_to_world(input.get_last_mouse_position());
				const vec2 mouseGlobalPosition = m_Camera.screen_to_world(movedEvent.get_position());
				const vec2 mouseDelta = mouseGlobalPosition - lastGlobalMousePosition;
				vec2 mouseRelativeDelta, up, right;

				switch(m_Space) {
				case GizmoSpace::Local: {
					const mat3 rotationMatrix = trans->get_rotation_matrix();
					mouseRelativeDelta = glm::inverse(rotationMatrix) * vec3(mouseGlobalPosition - lastGlobalMousePosition, 0.0f);
					up = trans->get_up_dir_ex(rotationMatrix);
					right = trans->get_right_dir_ex(rotationMatrix);
					break;
				}

				case GizmoSpace::Global:
					mouseRelativeDelta = mouseGlobalPosition - lastGlobalMousePosition;
					up = vec2(0.0f, 1.0f);
					right = vec2(1.0f, 0.0f);
					break;
				}

				switch(m_Mode) {
				default:
				case GizmoMode::Translate:
					if(mp_ActivePart == &m_XAxisPart)
						trans->position += right * mouseRelativeDelta.x;
					else if(mp_ActivePart == &m_YAxisPart)
						trans->position += up * mouseRelativeDelta.y;
					else if(mp_ActivePart == &m_OriginPart)
						trans->position += mouseDelta;
					break;

				case GizmoMode::Scale:
					const f32 multiplier = 0.01f;
					const vec2 value = mouseRelativeDelta * multiplier;
					if(mp_ActivePart == &m_XAxisPart)
						trans->scale.x += value.x;
					else if(mp_ActivePart == &m_YAxisPart)
						trans->scale.y += value.y;
					else if(mp_ActivePart == &m_OriginPart)
						trans->scale += (mouseDelta.x + mouseDelta.y) * multiplier;
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
		const vec2 mousePositionGlobal = cam.screen_to_world(InputModule::get_instance().get_mouse_position());
		const vec2 scaledHalfArrowSize = ARROW_SIZE * zoomFactor * 0.5f;
		const vec2 scaledHalfOriginSize = m_OriginPart.size * zoomFactor * 0.5f;
		const vec2 mousePositionLocal = m_Space == GizmoSpace::Local ? vec2(glm::inverse(trans.get_rotation_matrix()) * vec3(mousePositionGlobal - trans.position, 0.0f)) : (mousePositionGlobal - trans.position);

		if(mousePositionLocal.x >= -scaledHalfOriginSize.x && mousePositionLocal.x <= scaledHalfOriginSize.x && mousePositionLocal.y >= -scaledHalfOriginSize.y &&
		   mousePositionLocal.y <= scaledHalfOriginSize.y) {
			mp_HoveredPart = &m_OriginPart;
		} else if(mousePositionLocal.x >= -scaledHalfOriginSize.x && mousePositionLocal.x <= scaledHalfArrowSize.x && mousePositionLocal.y >= 0 && mousePositionLocal.y <= scaledHalfArrowSize.y * 2) {
			mp_HoveredPart = &m_YAxisPart;
		} else if(mousePositionLocal.x >= 0 && mousePositionLocal.x <= scaledHalfArrowSize.y * 2 && mousePositionLocal.y >= -scaledHalfArrowSize.x && mousePositionLocal.y <= scaledHalfArrowSize.x) {
			mp_HoveredPart = &m_XAxisPart;
		} else {
			mp_HoveredPart = nullptr;
		}
	}
}// namespace fifed
