#include "editorModule.hpp"
#include "components/cameraControllerComponent.hpp"

#include "fif/core/application.hpp"
#include "fif/core/ecs/entity.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/event/eventDispatcher.hpp"
#include "fif/core/event/mouseEvent.hpp"
#include "fif/core/performanceStats.hpp"
#include "fif/core/profiler.hpp"
#include "fif/core/util/rng.hpp"
#include "fif/gfx/components/renderableCircleComponent.hpp"
#include "fif/gfx/components/renderableQuadComponent.hpp"
#include "fif/gfx/components/transformComponent.hpp"
#include "fif/gfx/orthoCamera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/imGui/imGuiModule.hpp"
#include "fif/input/inputModule.hpp"

#include "imgui.h"

#include <cmath>
#include <functional>

EditorModule::EditorModule() {}
EditorModule::~EditorModule() {}

void EditorModule::onStart(fif::core::Application &app) {
	fif::imgui::ImGuiModule::getInstance()->addRenderFunc(&onRenderImGui);

	fif::core::Entity *cameraController = app.createEntity("CameraController");
	cameraController->addComponent<CameraControllerComponent>(fif::gfx::Renderer2D::getCamera());

	for (u32 i = 0; i < 1000; ++i) {
		fif::core::Entity *ent = app.createEntity("Test " + std::to_string(i));
		ent->addComponent<fif::gfx::TransformComponent>();

		fif::gfx::RenderableComponent *renderableComponent;
		if (fif::core::Rng::getBool()) {
			fif::gfx::RenderableQuadComponent *quad =
				ent->addComponent<fif::gfx::RenderableQuadComponent>();
			quad->m_Size = {fif::core::Rng::getF32(50, 100), fif::core::Rng::getF32(50, 100)};
			renderableComponent = quad;
		} else {
			fif::gfx::RenderableCircleComponent *circle =
				ent->addComponent<fif::gfx::RenderableCircleComponent>();
			circle->m_Radius = fif::core::Rng::getF32(50, 100);
			renderableComponent = circle;
		}

		renderableComponent->m_Color = {fif::core::Rng::getU8(0, 255u),
										fif::core::Rng::getU8(0, 255u),
										fif::core::Rng::getU8(0, 255u), 200};
		renderableComponent->mp_Transform->m_Position = {fif::core::Rng::getF32(-10000, 10000),
														 fif::core::Rng::getF32(-10000, 10000)};
	}
}

void EditorModule::onRenderImGui() {
	FIF_PROFILE_FUNC();
	if (ImGui::Begin("Performance")) {
		const PerformanceStats &stats = fif::core::Application::getInstance().getPerformanceStats();
		ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
		ImGui::Text("FPS: %f", stats.fps);

		if (ImGui::TreeNode("Renderer2D")) {
			const fif::gfx::Renderer2D::Stats &rendererStats = fif::gfx::Renderer2D::getStats();
			ImGui::Text("Circles: %i", rendererStats.circles);
			ImGui::Text("Quads: %i", rendererStats.quads);
			ImGui::Text("Vertices: %i", rendererStats.vertices);
			ImGui::Text("Elements: %i", rendererStats.elements);
			ImGui::TreePop();
		}
	}
	ImGui::End();

	if (ImGui::Begin("Entities")) {
		std::vector<fif::core::Entity> &entities =
			fif::core::Application::getInstance().getEntities();

		ImGui::Text("Count: %lu", entities.size());
		if (ImGui::BeginChild("EntityList")) {
			for (fif::core::Entity &ent : entities) {
				if (ImGui::TreeNode(ent.getName().c_str())) {
					if (ImGui::TreeNode("Components")) {
						for (const auto &comp : ent.getComponents())
							ImGui::Text("%s", comp->getName());
						ImGui::TreePop();
					}

					if (ImGui::Button("Delete"))
						ent.queueDelete();

					ImGui::TreePop();
				}
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();
}
