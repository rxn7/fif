#include "editorModule.h"
#include "components/cameraControllerComponent.h"
#include "fif/core/application.h"
#include "fif/core/ecs/entity.h"
#include "fif/core/event/event.h"
#include "fif/core/event/eventDispatcher.h"
#include "fif/core/event/mouseEvent.h"
#include "fif/core/performanceStats.h"
#include "fif/core/profiler.h"
#include "fif/core/scopeTimer.h"
#include "fif/gfx/components/renderableCircleComponent.h"
#include "fif/gfx/components/renderableQuadComponent.h"
#include "fif/gfx/components/transformComponent.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/renderer2d.h"
#include "fif/imGui/imGuiModule.h"

#include "fif/input/inputModule.h"
#include "imgui.h"

#include <cmath>

EditorModule::EditorModule() {}

EditorModule::~EditorModule() {}

void EditorModule::onStart(fif::core::Application &app) {
    fif::imgui::ImGuiModule::getInstance()->addRenderFunc(std::bind(&EditorModule::onRenderImGui, this));

    fif::core::Entity *cameraController = app.createEntity();
    cameraController->addComponent<CameraControllerComponent>(fif::gfx::Renderer2D::getCamera());

    for (std::uint32_t i = 0; i < 1000; ++i) {
        fif::core::Entity *ent = app.createEntity();
        ent->addComponent<fif::gfx::TransformComponent>();

        fif::gfx::RenderableComponent *renderableComponent;
        if (rand() % 2 == 0) {
            fif::gfx::RenderableQuadComponent *quad = ent->addComponent<fif::gfx::RenderableQuadComponent>();
            quad->m_Size = {100, 100};
            quad->m_Color = {rand() % 255, rand() % 255, rand() % 255, 200};
            renderableComponent = quad;
        } else {
            fif::gfx::RenderableCircleComponent *circle = ent->addComponent<fif::gfx::RenderableCircleComponent>();
            circle->m_Diameter = 100.0f;
            circle->m_Color = {rand() % 255, rand() % 255, rand() % 255, 200};
            renderableComponent = circle;
        }

        renderableComponent->mp_Transform->m_Position = {std::rand() % 10000 - 5000, std::rand() % 10000 - 5000};
    }
}

void EditorModule::onRenderImGui() {
    FIF_PROFILE_FUNC();
    if (ImGui::Begin("Performance")) {
        {
            const PerformanceStats &stats = fif::core::Application::getInstance().getLastFramePerformanceStats();
            ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
            ImGui::Text("FPS: %f", stats.fps);
        }

        if (ImGui::TreeNode("Profiler")) {
            const std::vector<fif::core::TimerResult> profilerResults = fif::core::Profiler::getResults();

            for (const fif::core::TimerResult &result : profilerResults)
                ImGui::Text("%s: %f ms", result.name.c_str(), result.durationMs);

            ImGui::TreePop();
        }
    }
    ImGui::End();

    if (ImGui::Begin("Entities")) {
        const std::vector<fif::core::Entity> &entities = fif::core::Application::getInstance().getEntities();

        ImGui::Text("Count: %lu", entities.size());
        std::uint32_t i = 1;
        for (const fif::core::Entity &ent : entities) {
            if (ImGui::TreeNode(std::to_string(i++).c_str())) {
                for (const auto &comp : ent.getComponents())
                    ImGui::Text("%s", comp->getName());

                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}
