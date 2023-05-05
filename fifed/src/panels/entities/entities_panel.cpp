#include "entities_panel.hpp"

#include "components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/transform_component.hpp"

#include "fif/core/ecs/components/tag_component.hpp"

namespace fifed {
	const char *EntitiesPanel::get_title() const {
		return "Entities";
	}

	void EntitiesPanel::on_render() {
		Scene &scene = Application::get_instance().get_scene();

		ImGui::Text("Count: %lu", scene.get_entity_count());
		if(ImGui::Button("Create new")) {
			EntityID ent = scene.create_entity();

			TransformComponent &trans = scene.add_component<TransformComponent>(ent);
			trans.position = Rng::get_vec2(-1000, 1000);

			if(Rng::get_bool()) {
				QuadComponent &quad = scene.add_component<QuadComponent>(ent);
				quad.size = glm::vec2(Rng::get_f32(20, 100), Rng::get_f32(20, 100));
			} else {
				CircleComponent &circle = scene.add_component<CircleComponent>(ent);
				circle.radius = Rng::get_f32(40, 200);
			}

			RenderableComponent &renderable = scene.add_component<RenderableComponent>(ent);
			renderable.color = Color(Rng::get_u8(0u, 255u), Rng::get_u8(0u, 255u), Rng::get_u8(0u, 255u), Rng::get_u8(100u, 255u));
		}

		if(ImGui::BeginChild("EntityList")) {
			scene.for_each([&](EntityID &ent) {
				const char *name;

				if(scene.has_component<TagComponent>(ent))
					name = scene.get_component<TagComponent>(ent).tag;
				else
					name = std::to_string(static_cast<u32>(ent)).c_str();

				if(ImGui::TreeNode(name)) {
					if(ImGui::TreeNode("Components")) {
						// TODO: Render components properties
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
}// namespace fifed
