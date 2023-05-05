#include "entities_panel.hpp"

#include "components/circle_component.hpp"
#include "components/lua_script_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/transform_component.hpp"

#include "fif/core/ecs/components/tag_component.hpp"
#include "imgui.h"
#include "lua_scripting_module.hpp"

namespace fifed {
	template<typename T> static void draw_component(const std::string &name, EntityID ent, Scene &scene, std::function<void(T &)> drawFunc);

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

			QuadComponent &quad = scene.add_component<QuadComponent>(ent);
			quad.size = glm::vec2(Rng::get_f32(20, 100), Rng::get_f32(20, 100));

			RenderableComponent &renderable = scene.add_component<RenderableComponent>(ent);
			renderable.color = Color(Rng::get_u8(0u, 255u), Rng::get_u8(0u, 255u), Rng::get_u8(0u, 255u), Rng::get_u8(100u, 255u));

			scene.add_component<LuaScriptComponent>(ent);
		}

		if(ImGui::BeginChild("EntityList"))
			scene.for_each([&](EntityID &ent) { draw_entity(ent, scene); });

		ImGui::EndChild();
	}

	void EntitiesPanel::draw_components(EntityID ent, Scene &scene) {
		draw_component<TransformComponent>("Transform", ent, scene, [](TransformComponent &transform) {
			ImGui::DragFloat2("Position", glm::value_ptr(transform.position));

			float angleDegrees = glm::degrees(transform.angle);
			ImGui::DragFloat("Angle", &angleDegrees, 1.0f, 0.0f, 360.0f);
			transform.angle = glm::radians(angleDegrees);
		});

		draw_component<RenderableComponent>("Renderable", ent, scene, [](RenderableComponent &renderable) {
			glm::vec4 color = glm::vec4(renderable.color) * (1.0f / 255.0f);
			ImGui::ColorEdit4("Color", glm::value_ptr(color));
			renderable.color = Color((color * 255.0f));
		});

		draw_component<QuadComponent>("Quad", ent, scene, [](QuadComponent &quad) { ImGui::DragFloat2("Size", glm::value_ptr(quad.size)); });

		draw_component<CircleComponent>("Circle", ent, scene, [](CircleComponent &circle) {
			ImGui::DragFloat("Radius", &circle.radius, 1.0f, 0.0f, 1000.0f);
			constexpr u16 MIN_SEGMENTS = 5;
			constexpr u16 MAX_SEGMENTS = 1000;
			ImGui::DragScalar("Segments", ImGuiDataType_U16, &circle.segments, 1, &MIN_SEGMENTS, &MAX_SEGMENTS);
		});

		draw_component<LuaScriptComponent>("Lua Script", ent, scene, [](LuaScriptComponent &script) {
			if(script.loaded)
				ImGui::Text("Script: %s", script.path);
			else
				ImGui::Text("No script is attached to this component!");

			if(ImGui::Button("Load Script"))
				ImGui::OpenPopup("LoadScript");

			if(ImGui::BeginPopup("LoadScript")) {
				ImGui::InputText("File path", script.path, 1024);
				if(ImGui::Button("Load")) {
					LuaScriptingModule::get_instance()->attach_script(script, script.path);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		});
	}

	void EntitiesPanel::draw_entity(EntityID ent, Scene &scene) {
		const char *name = scene.has_component<TagComponent>(ent) ? scene.get_component<TagComponent>(ent).tag.c_str() : "Entity";

		ImGui::PushID(static_cast<u32>(ent));

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		bool open = ImGui::TreeNodeEx(name, flags);

		if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
			ImGui::OpenPopup("EntitySettings");

		bool deleteEntity = false;
		if(ImGui::BeginPopup("EntitySettings")) {
			deleteEntity = ImGui::MenuItem("Delete entity");
			ImGui::EndPopup();
		}

		if(open) {
			draw_components(ent, scene);
			ImGui::TreePop();
		}

		ImGui::PopID();

		if(deleteEntity)
			scene.delete_entity(ent);
	}

	template<typename T> static void draw_component(const std::string &name, EntityID ent, Scene &scene, std::function<void(T &)> drawFunc) {
		T *comp = scene.get_registry().try_get<T>(ent);
		if(!comp)
			return;

		ImGui::PushID(typeid(T).hash_code() + static_cast<u32>(ent));

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		bool open = ImGui::TreeNodeEx(name.c_str(), flags);

		if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
			ImGui::OpenPopup("ComponentSettings");

		bool removeComponent = false;
		if(ImGui::BeginPopup("ComponentSettings")) {
			removeComponent = ImGui::MenuItem("Remove component");

			ImGui::EndPopup();
		}

		if(open) {
			drawFunc(*comp);
			ImGui::TreePop();
		}

		if(removeComponent)
			scene.remove_component<T>(ent);

		ImGui::PopID();
	}

}// namespace fifed
