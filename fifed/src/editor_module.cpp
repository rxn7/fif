#include "editor_module.hpp"
#include "components/camera_controller_component.hpp"

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

#include "imgui.h"

EditorModule::EditorModule() {}
EditorModule::~EditorModule() {}

void EditorModule::on_start(fif::core::Application &app) {
	app.mp_Scene = mp_Scene = std::make_shared<EditorScene>();

	fif::imgui::ImGuiModule::get_instance()->add_render_func(std::bind(&EditorModule::on_render_im_gui, this));

	fif::core::Entity *cameraController = mp_Scene->create_editor_entity("CameraController");
	cameraController->add_component<CameraControllerComponent>(fif::gfx::Renderer2D::get_camera());

	for(u32 i = 0; i < 1000; ++i) {
		fif::core::Entity *ent = app.mp_Scene->create_entity("Test " + std::to_string(i));
		ent->add_component<fif::gfx::TransformComponent>();

		fif::gfx::RenderableComponent *renderableComponent;
		if(fif::core::Rng::get_bool()) {
			fif::gfx::RenderableQuadComponent *quad = ent->add_component<fif::gfx::RenderableQuadComponent>();
			quad->m_Size = {fif::core::Rng::get_f32(50, 100), fif::core::Rng::get_f32(50, 100)};
			renderableComponent = quad;
		} else {
			fif::gfx::RenderableCircleComponent *circle = ent->add_component<fif::gfx::RenderableCircleComponent>();
			circle->m_Radius = fif::core::Rng::get_f32(50, 100);
			renderableComponent = circle;
		}

		renderableComponent->m_Color = {fif::core::Rng::get_u8(0, 255u), fif::core::Rng::get_u8(0, 255u), fif::core::Rng::get_u8(0, 255u), 200};
		renderableComponent->mp_Transform->m_Position = {fif::core::Rng::get_f32(-10000, 10000), fif::core::Rng::get_f32(-10000, 10000)};
	}
}

void EditorModule::on_render_im_gui() {
	FIF_PROFILE_FUNC();
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

	if(ImGui::Begin("Entities")) {
		ImGui::Text("Count: %lu", mp_Scene->get_entity_count());
		if(ImGui::BeginChild("EntityList")) {
			mp_Scene->for_each(
				[&](fif::core::Entity &ent) {
					if(ImGui::TreeNode(ent.get_name().c_str())) {
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
			ImGui::EndChild();
		}
	}
	ImGui::End();
}
