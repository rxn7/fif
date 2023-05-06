#include "inspector_panel.hpp"
#include "components/circle_component.hpp"
#include "components/quad_component.hpp"
#include "components/renderable_component.hpp"
#include "components/transform_component.hpp"
#include "ecs/components/tag_component.hpp"
#include "imgui.h"

namespace fifed {
	const char *InspectorPanel::get_title() const {
		return "Inspector";
	}

	template<typename T, typename... Args> static void draw_add_component_entry(const std::string &name, EntityID ent, Scene &scene, Args &&...args) {
		if(scene.has_component<T>(ent))
			return;

		if(ImGui::MenuItem(name.c_str())) {
			scene.add_component<T>(ent, args...);
			ImGui::CloseCurrentPopup();
		}
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

	void InspectorPanel::on_render() {
		if(m_SelectedEntity == entt::null)
			return;

		Scene &scene = Application::get_instance()->get_scene();

		if(ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if(ImGui::BeginPopup("AddComponent")) {
			draw_add_component_entry<TagComponent>("Tag", m_SelectedEntity, scene, "Entity");
			draw_add_component_entry<TransformComponent>("Transform", m_SelectedEntity, scene);
			draw_add_component_entry<RenderableComponent>("Renderable", m_SelectedEntity, scene);
			draw_add_component_entry<QuadComponent>("Quad", m_SelectedEntity, scene);
			draw_add_component_entry<CircleComponent>("Circle", m_SelectedEntity, scene);
			draw_add_component_entry<LuaScriptComponent>("Lua Script", m_SelectedEntity, scene);
			ImGui::EndPopup();
		}

		ImGui::Separator();

		if(TagComponent *tag = scene.get_registry().try_get<TagComponent>(m_SelectedEntity)) {
			std::strncpy(m_TagBuffer.data(), tag->tag.c_str(), tag->tag.size());
			if(ImGui::InputText("Tag", m_TagBuffer.data(), m_TagBuffer.size()))
				tag->tag = m_TagBuffer.data();
		}

		draw_component<TransformComponent>("Transform", m_SelectedEntity, scene, [](TransformComponent &transform) {
			ImGui::DragFloat2("Position", glm::value_ptr(transform.position));

			float angleDegrees = glm::degrees(transform.angle);
			ImGui::DragFloat("Angle", &angleDegrees, 1.0f, 0.0f, 360.0f);
			transform.angle = glm::radians(angleDegrees);
		});

		draw_component<RenderableComponent>("Renderable", m_SelectedEntity, scene, [](RenderableComponent &renderable) {
			glm::vec4 color = get_normalized_color(renderable.color);
			ImGui::ColorEdit4("Color", glm::value_ptr(color));
			renderable.color = get_color_from_normalized(color);
		});

		draw_component<QuadComponent>(
			"Quad", m_SelectedEntity, scene, [](QuadComponent &quad) { ImGui::DragFloat2("Size", glm::value_ptr(quad.size)); });

		draw_component<CircleComponent>("Circle", m_SelectedEntity, scene, [](CircleComponent &circle) {
			ImGui::DragFloat("Radius", &circle.radius, 1.0f, 0.0f, 1000.0f);
			constexpr u16 MIN_SEGMENTS = 5;
			constexpr u16 MAX_SEGMENTS = 1000;
			ImGui::DragScalar("Segments", ImGuiDataType_U16, &circle.segments, 1, &MIN_SEGMENTS, &MAX_SEGMENTS);
		});

		draw_component<LuaScriptComponent>("Lua Script", m_SelectedEntity, scene, [](LuaScriptComponent &script) {
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
}// namespace fifed
