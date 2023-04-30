#include "editor_module.hpp"
#include "color.hpp"
#include "components/camera_controller_component.hpp"
#include "components/grid_renderer_component.hpp"

#include "fif/core/application.hpp"
#include "fif/core/ecs/entity.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/mouse_event.hpp"
#include "fif/core/performance_stats.hpp"
#include "fif/core/profiler.hpp"
#include "fif/core/util/rng.hpp"
#include "fif/gfx/components/renderable_circle_component.hpp"
#include "fif/gfx/components/renderable_quad_component.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/imgui/imgui_module.hpp"
#include "fif/input/input_module.hpp"

#include "gfx_module.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include <limits>

EditorModule::EditorModule() {}
EditorModule::~EditorModule() {}

void EditorModule::on_start(fif::core::Application &app) {
	mp_FrameBuffer = std::make_unique<fif::gfx::FrameBuffer>(app.get_window().get_size());
	app.mp_Scene = mp_Scene = std::make_shared<EditorScene>();

	fif::imgui::ImGuiModule::get_instance()->add_render_func(std::bind(&EditorModule::on_render_im_gui, this));

	mp_EditorEntity = mp_Scene->create_editor_entity();
	mp_CameraControllerComponent = mp_EditorEntity->add_component<CameraControllerComponent>();
	mp_GridComponent = mp_EditorEntity->add_component<GridRendererComponent>();
}

void EditorModule::on_render_im_gui() {
	FIF_PROFILE_FUNC();

	// TODO: abstract EditorPanel class

	if(fif::imgui::ImGuiModule::get_instance()->begin_dockspace()) {
		if(ImGui::Begin("Performance")) {
			const PerformanceStats &stats = fif::core::Application::get_instance().get_performance_stats();
			ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
			ImGui::Text("FPS: %f", stats.fps);

			if(ImGui::TreeNode("Renderer2D")) {
				const fif::gfx::Renderer2D::Stats &rendererStats = fif::gfx::Renderer2D::get_stats();
				ImGui::Text("Circles: %i", rendererStats.circles);
				ImGui::Text("Quads: %i", rendererStats.quads);
				ImGui::Text("Vertices: %i", rendererStats.vertices);
				ImGui::Text("Elements: %i", rendererStats.elements);
				ImGui::TreePop();
			}
		}
		ImGui::End();

		if(ImGui::Begin("Settings")) {
			if(ImGui::TreeNode("Grid")) {
				ImGui::Checkbox("Enabled", &mp_GridComponent->m_Enabled);
				ImGui::SliderFloat("Line tickness", &mp_GridComponent->m_LineThickness, 0.0f, 1.0f);
				ImGui::SliderFloat("Cell size", &mp_GridComponent->m_CellSize, 0.1f, 100.0f);

				glm::vec4 colorNormalized = fif::gfx::get_normalized_color(mp_GridComponent->m_LineColor);
				ImGui::ColorEdit4("Line color", glm::value_ptr(colorNormalized));
				mp_GridComponent->m_LineColor = fif::gfx::get_color_from_normalized(colorNormalized);
				ImGui::SliderFloat("Wrap Value", &mp_GridComponent->m_WrapValue, 1.0f, 1000.0f);

				ImGui::TreePop();
			}
			if(ImGui::TreeNode("Camera controller")) {
				ImGui::SliderFloat("Zoom lerp speed", &mp_CameraControllerComponent->m_ZoomLerpSpeed, 0.1f, 100.0f);
				ImGui::SliderFloat("Min zoom", &mp_CameraControllerComponent->m_MinZoom, 0.001f, 0.1f);
				ImGui::SliderFloat("Max zoom", &mp_CameraControllerComponent->m_MaxZoom, 1.0f, 1000.0f);

				ImGui::TreePop();
			}
		}
		ImGui::End();

		if(ImGui::Begin("Entities")) {
			ImGui::Text("Count: %lu", mp_Scene->get_entity_count());
			if(ImGui::Button("Create new")) {
				fif::core::Entity *ent = mp_Scene->create_entity();

				fif::gfx::TransformComponent *trans = ent->add_component<fif::gfx::TransformComponent>();
				trans->m_Position = fif::core::Rng::get_vec2(-1000, 1000);

				fif::gfx::RenderableCircleComponent *circle = ent->add_component<fif::gfx::RenderableCircleComponent>();
				circle->m_Radius = fif::core::Rng::get_f32(20, 100);
				circle->m_Color = fif::gfx::Color(fif::core::Rng::get_u8(0u, 255u), fif::core::Rng::get_u8(0u, 255u), fif::core::Rng::get_u8(0u, 255u), fif::core::Rng::get_u8(100u, 255u));
			}

			if(ImGui::BeginChild("EntityList")) {
				mp_Scene->for_each(
					[&](fif::core::Entity &ent) {
						if(ImGui::TreeNode(std::to_string(ent.get_uuid()).c_str())) {
							if(ImGui::TreeNode("Components")) {
								for(const auto &comp : ent.get_components())
									ImGui::Text("%s", comp->get_name());
								ImGui::TreePop();
							}

							if(ImGui::Button("Delete"))
								ent.queue_delete();

							ImGui::TreePop();
						}
					},
					true);
			}
			ImGui::EndChild();
		}
		ImGui::End();

		if(ImGui::Begin("Viewport")) {
			ImGui::BeginChild("FrameBuffer");

			const ImVec2 pos = ImGui::GetWindowPos();
			const ImVec2 size = ImGui::GetWindowSize();

			fif::gfx::GfxModule::set_viewport(glm::vec2(size.x, size.y), glm::vec2(pos.x, pos.y));
			mp_FrameBuffer->set_size(glm::vec2(size.x, size.y));

			ImGui::Image(reinterpret_cast<ImTextureID>(mp_FrameBuffer->getTextureID()), size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
			ImGui::EndChild();
		}
		ImGui::End();
	}
	ImGui::End();
}

void EditorModule::on_update() {
	mp_FrameBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
}

void EditorModule::on_render() {
	mp_FrameBuffer->unbind();
}
