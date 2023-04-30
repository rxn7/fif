#include "editor_module.hpp"
#include "camera_controller.hpp"
#include "color.hpp"
#include "common.hpp"
#include "components/lua_script_component.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"
#include "grid.hpp"

#include "components/quad_component.hpp"
#include "components/renderable_component.hpp"
#include "ecs/scene.hpp"
#include "fif/core/application.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/mouse_event.hpp"
#include "fif/core/performance_stats.hpp"
#include "fif/core/util/rng.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "fif/gfx/gfx_module.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/imgui/imgui_module.hpp"
#include "fif/input/input_module.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include <limits>

EditorModule::EditorModule() {}
EditorModule::~EditorModule() {}

void EditorModule::on_start(Application &app) {
	mp_FrameBuffer = std::make_unique<FrameBuffer>(app.get_window().get_size());

	ImGuiModule::get_instance()->add_render_func(std::bind(&EditorModule::on_render_im_gui, this));

	Grid::init();
}

void EditorModule::on_render_im_gui() {
	// TODO: abstract EditorPanel class

	if(ImGuiModule::get_instance()->begin_dockspace()) {
		if(ImGui::Begin("Performance")) {
			const PerformanceStats &stats = Application::get_instance().get_performance_stats();
			ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
			ImGui::Text("FPS: %f", stats.fps);

			if(ImGui::TreeNode("Renderer2D")) {
				const Renderer2D::Stats &rendererStats = Renderer2D::get_stats();
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
				ImGui::Checkbox("Enabled", &Grid::enabled);
				ImGui::SliderFloat("Line tickness", &Grid::lineThickness, 0.0f, 1.0f);
				ImGui::SliderFloat("Cell size", &Grid::cellSize, 0.1f, 100.0f);

				glm::vec4 colorNormalized = get_normalized_color(Grid::lineColor);
				ImGui::ColorEdit4("Line color", glm::value_ptr(colorNormalized));
				Grid::lineColor = get_color_from_normalized(colorNormalized);
				ImGui::SliderFloat("Wrap Value", &Grid::wrapValue, 1.0f, 1000.0f);

				ImGui::TreePop();
			}
			if(ImGui::TreeNode("Camera controller")) {
				ImGui::SliderFloat("Zoom lerp speed", &CameraController::zoomLerpSpeed, 0.1f, 100.0f);
				ImGui::SliderFloat("Min zoom", &CameraController::minZoom, 0.001f, 0.1f);
				ImGui::SliderFloat("Max zoom", &CameraController::maxZoom, 1.0f, 1000.0f);

				ImGui::TreePop();
			}
		}
		ImGui::End();

		if(ImGui::Begin("Entities")) {
			Scene &scene = Application::get_instance().get_scene();

			ImGui::Text("Count: %lu", scene.get_entity_count());
			if(ImGui::Button("Create new")) {
				EntityID ent = scene.create_entity();

				TransformComponent &trans = scene.add_component<TransformComponent>(ent);
				trans.position = Rng::get_vec2(-1000, 1000);

				QuadComponent &quad = scene.add_component<QuadComponent>(ent);
				quad.size = glm::vec2(Rng::get_f32(20, 100), Rng::get_f32(20, 100));

				RenderableComponent &renderable = scene.add_component<RenderableComponent>(ent);
				renderable.color = Color(Rng::get_u8(0u, 255u), Rng::get_u8(0u, 255u), Rng::get_u8(0u, 255u), Rng::get_u8(100u, 255u));

				LuaScriptComponent &script = scene.add_component<LuaScriptComponent>(ent);
				LuaScriptingModule::get_instance()->attach_script(script, "assets/scripts/test.lua");
			}

			if(ImGui::BeginChild("EntityList")) {
				scene.for_each([&](EntityID &ent) {
					if(ImGui::TreeNode(std::to_string(static_cast<u32>(ent)).c_str())) {
						if(ImGui::TreeNode("Components")) {
							// TODO:
							ImGui::TreePop();
						}

						if(ImGui::Button("Delete"))
							scene.delete_entity(ent);

						ImGui::TreePop();
					}
				});
			}
			ImGui::EndChild();
		}
		ImGui::End();

		if(ImGui::Begin("Viewport")) {
			ImGui::BeginChild("FrameBuffer");

			const ImVec2 pos = ImGui::GetWindowPos();
			const ImVec2 size = ImGui::GetWindowSize();

			GfxModule::set_viewport(glm::vec2(size.x, size.y), glm::vec2(pos.x, pos.y));
			mp_FrameBuffer->set_size(glm::vec2(size.x, size.y));

			ImGui::Image(reinterpret_cast<ImTextureID>(mp_FrameBuffer->getTextureID()), size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
			ImGui::EndChild();
		}
		ImGui::End();
	}
	ImGui::End();
}

void EditorModule::on_update() {
	CameraController::update();

	mp_FrameBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	Grid::render();
}

void EditorModule::on_render() {
	mp_FrameBuffer->unbind();
}

void EditorModule::on_event(Event &event) {
	CameraController::on_event(event);
}
