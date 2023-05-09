#include "scene_panel.hpp"

#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "fif/native_scripting/components/native_script_component.hpp"

#include "imgui.h"
#include "lua_scripting_module.hpp"

namespace fifed {
	template<typename T> static void draw_component(const std::string &name, EntityID ent, Scene &scene, std::function<void(T &)> drawFunc);

	ScenePanel::ScenePanel(InspectorPanel &inspector) : m_Inspector(inspector) {}

	void ScenePanel::on_render() {
		Scene &scene = Application::get_instance()->get_scene();

		ImGui::Text("Count: %lu", scene.get_entity_count());

		if(ImGui::Button("Create entity")) {
			const EntityID ent = scene.create_entity();
			scene.add_component<TransformComponent>(ent);
			m_Inspector.m_SelectedEntity = ent;
		}

		if(ImGui::BeginChild("EntityList"))
			scene.for_each([&](EntityID &ent) { draw_entity(ent, scene); });

		ImGui::EndChild();
	}

	void ScenePanel::draw_entity(EntityID ent, Scene &scene) {
		const char *name = scene.has_component<TagComponent>(ent) ? scene.get_component<TagComponent>(ent).tag.c_str() : "Entity";

		ImGui::PushID(static_cast<u32>(ent));

		const bool isSelected = m_Inspector.m_SelectedEntity == ent;

		// TODO: Remove ImGuiTreeNodeFlags_Leaf if entity has children
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_OpenOnArrow | (isSelected ? ImGuiTreeNodeFlags_Selected : 0);

		const bool open = ImGui::TreeNodeEx(name, flags);
		bool deleteEntity = false;

		if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
			ImGui::OpenPopup("EntitySettings");

		if(ImGui::IsItemClicked(ImGuiMouseButton_Left))
			m_Inspector.m_SelectedEntity = ent;

		if(ImGui::BeginPopup("EntitySettings")) {
			deleteEntity = ImGui::MenuItem("Delete entity");
			ImGui::EndPopup();
		}

		if(open) {
			// TODO: draw children entities
			ImGui::TreePop();
		}

		ImGui::PopID();

		if(deleteEntity) {
			scene.delete_entity(ent);
			if(m_Inspector.m_SelectedEntity == ent)
				m_Inspector.m_SelectedEntity = entt::null;
		}
	}
}// namespace fifed
