#include "scene_panel.hpp"
#include "entity_template/empty_template.hpp"
#include "entity_template/sprite_template.hpp"

#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "fif/native_scripting/components/native_script_component.hpp"

#include "imgui.h"
#include "lua_scripting_module.hpp"

namespace fifed {
	template<typename T> static void draw_component(const std::string &name, EntityID ent, Scene &scene, std::function<void(T &)> drawFunc);

	ScenePanel::ScenePanel(InspectorPanel &inspector) : m_Inspector(inspector) {
		m_Templates.push_back(std::make_unique<EmptyTemplate>());
		m_Templates.push_back(std::make_unique<SpriteTemplate>());
	}

	void ScenePanel::on_render() {
		Scene &scene = Application::get_instance()->get_scene();

		const u32 entCount = scene.get_entity_count();
		ImGui::Text("Count: %u", entCount);

		if(ImGui::Button("Create entity"))
			ImGui::OpenPopup("SelectTemplate");

		if(ImGui::BeginPopup("SelectTemplate")) {
			for(auto &tmplt : m_Templates) {
				if(ImGui::MenuItem(tmplt->get_name().data())) {
					m_Inspector.m_SelectedEntity = tmplt->create(scene);
					break;
				}
			}
			ImGui::EndPopup();
		}

		if(ImGui::BeginChild("EntityList")) {
			const auto &view = scene.get_registry().view<TagComponent>();

			ImGuiListClipper clipper;
			clipper.Begin(view.size());

			while(clipper.Step()) {
				for(i32 i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
					EntityID ent = *std::next(view.begin(), i);
					const TagComponent &tag = std::get<0>(view.get(ent));
					draw_entity(ent, tag.tag.c_str(), scene);
				}
			}

			clipper.End();
		}
		ImGui::EndChild();
	}

	void ScenePanel::draw_entity(EntityID ent, const char *name, Scene &scene) {
		ImGui::PushID(static_cast<u32>(ent));

		const bool isSelected = m_Inspector.m_SelectedEntity == ent;
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

		if(deleteEntity) {
			scene.delete_entity(ent);
			if(m_Inspector.m_SelectedEntity == ent)
				m_Inspector.m_SelectedEntity = entt::null;
		}

		if(open) {
			// TODO: draw children entities
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}// namespace fifed
