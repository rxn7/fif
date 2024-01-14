#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class InspectorPanel final : public EditorPanel {
	public:
		InspectorPanel(Editor &editor);
		PANEL_NAME("Inspector")

		void on_render() override;

	private:
		static void draw_z_index_selector(i8 &zIndex);
		static void draw_color_selector(Color &color);

		template<typename T, typename... Args> void draw_add_component_entry(const std::string &name, std::function<void(T &comp)> callback = nullptr, Args &&...args) {
			if(m_Editor.m_SelectedEntity.has_component<T>())
				return;

			if(ImGui::MenuItem(name.c_str())) {
				T &comp = m_Editor.m_SelectedEntity.add_component<T>(std::forward<Args>(args)...);
				if(callback != nullptr)
					callback(comp);

				ImGui::CloseCurrentPopup();
			}
		}

		template<typename T> void draw_component(const std::string &name, std::function<void(T &)> drawFunc) {
			T *comp = m_Editor.m_SelectedEntity.try_get_component<T>();
			if(!comp)
				return;

			ImGui::PushID(typeid(T).hash_code() + static_cast<u32>(m_Editor.m_SelectedEntity.m_ID));

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
				m_Editor.m_SelectedEntity.remove_component<T>();

			ImGui::Separator();
			ImGui::PopID();
		}

	public:
		std::array<char, 512> m_TextBuffer;
	};
}// namespace fifed
