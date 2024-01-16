#include "inspector_panel.hpp"
#include "editor.hpp"

#include <fif/core/ecs/components/tag_component.hpp>
#include <fif/core/ecs/components/transform_component.hpp>
#include <fif/core/project.hpp>
#include <fif/gfx/components/circle_component.hpp>
#include <fif/gfx/components/label_component.hpp>
#include <fif/gfx/components/quad_component.hpp>
#include <fif/gfx/components/sprite_component.hpp>
#include <fif/gfx/resource/font.hpp>
#include <fif/lua_scripting/components/lua_script_component.hpp>
#include <fif/lua_scripting/lua_scripting_module.hpp>
#include <fif/native_scripting/components/native_script_component.hpp>

#include <filesystem>
#include <optional>
#include <tinyfiledialogs.h>

namespace fifed {
	InspectorPanel::InspectorPanel(Editor &editor) : EditorPanel(editor) {}

	void InspectorPanel::on_render() {
		if(m_Editor.m_SelectedEntity.m_ID == entt::null)
			return;

		if(ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if(ImGui::BeginPopup("AddComponent")) {
			draw_add_component_entry<TagComponent>("Tag");
			draw_add_component_entry<TransformComponent>("Transform");
			draw_add_component_entry<SpriteComponent>("Sprite");
			draw_add_component_entry<QuadComponent>("Quad");
			draw_add_component_entry<CircleComponent>("Circle");
			draw_add_component_entry<LabelComponent>("Label");
			draw_add_component_entry<LuaScriptComponent>("LuaScript", nullptr, m_Editor.m_SelectedEntity);
			ImGui::EndPopup();
		}

		ImGui::Spacing();

		if(TagComponent *tag = m_Editor.m_SelectedEntity.try_get_component<TagComponent>()) {
			std::strncpy(m_TextBuffer.data(), tag->tag.c_str(), tag->tag.size() + 1);
			if(ImGui::InputText("Tag", m_TextBuffer.data(), m_TextBuffer.size()))
				tag->tag = m_TextBuffer.data();
		}

		draw_component<TransformComponent>("Transform", [](TransformComponent &transform) {
			ImGui::DragFloat2("Position", glm::value_ptr(transform.position));
			ImGui::DragFloat2("Scale", glm::value_ptr(transform.scale), 0.1f);

			float angleDegrees = glm::degrees(transform.angleRadians);
			ImGui::DragFloat("Angle", &angleDegrees, 1.0f);
			transform.angleRadians = glm::radians(angleDegrees);
		});

		draw_component<QuadComponent>("Quad", [](QuadComponent &quad) {
			draw_z_index_selector(quad.zIndex);
			draw_color_selector(quad.tint);
			ImGui::DragFloat2("Size", glm::value_ptr(quad.size));
		});

		draw_component<SpriteComponent>("Sprite", [](SpriteComponent &sprite) {
			draw_z_index_selector(sprite.zIndex);

			draw_color_selector(sprite.tint);
			ImGui::DragFloat2("Size", glm::value_ptr(sprite.size), 1.0f, 0.0f, std::numeric_limits<float>::max());

			ImGui::Separator();

			ImGui::Text("Texture: ");
			if(sprite.p_Texture) {
				ImGui::Image(reinterpret_cast<ImTextureID>(sprite.p_Texture->get_id()), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
			} else {
				ImGui::SameLine();
				ImGui::Text("No texture is selected");
			}

			if(ImGui::Button("Change texture")) {
				static constexpr std::array<const char *, 3> filters = {"*.png", "*.jpg", "*.gif"};

				const char *fileDialogResult = tinyfd_openFileDialog("Select tetxure", Project::get_absolute_path().c_str(), filters.size(), filters.data(), "Texture", false);
				if(!fileDialogResult)
					return;

				const std::filesystem::path path = std::filesystem::relative(fileDialogResult, Project::get_root_dir());

				std::shared_ptr<Texture> texture = Project::get_resource_manager().add_resource<Texture>(path);
				if(!texture)
					return;

				sprite.p_Texture = texture;
				sprite.size = texture->get_size();
			}
		});

		draw_component<CircleComponent>("Circle", [](CircleComponent &circle) {
			draw_z_index_selector(circle.zIndex);

			draw_color_selector(circle.tint);
			ImGui::DragFloat("Radius", &circle.radius, 1.0f, 0.0f, std::numeric_limits<float>::max());
		});

		draw_component<LabelComponent>("Label", [&](LabelComponent &label) {
			draw_z_index_selector(label.zIndex);

			std::strncpy(m_TextBuffer.data(), label.text.c_str(), label.text.size() + 1);
			if(ImGui::InputText("Text", m_TextBuffer.data(), m_TextBuffer.size()))
				label.text = m_TextBuffer.data();

			ImGui::DragFloat("Size", &label.fontSize, 0.05f, 0.0f, std::numeric_limits<float>::max());
			draw_color_selector(label.color);

			{
				static const char *items[] = {"Left", "Center", "Right"};
				ImGui::Combo("Horizontal Align", (int *)&label.horizontalAlign, items, IM_ARRAYSIZE(items));
			}

			{
				static const char *items[] = {"Top", "Center", "Bottom"};
				ImGui::Combo("Vertical Align", (int *)&label.verticalAlign, items, IM_ARRAYSIZE(items));
			}

			ImGui::Separator();

			ImGui::Text("Font: ");
			if(label.p_font) {
				ImGui::Image(reinterpret_cast<ImTextureID>(label.p_font->get_texture()->get_id()), {256, 256});
			} else {
				ImGui::SameLine();
				ImGui::Text("Font is not selected");
			}

			if(ImGui::Button("Change font")) {
				static constexpr std::array<const char *, 1> filters = {"*.ttf"};

				const char *fileDialogResult = tinyfd_openFileDialog("Select font", Project::get_absolute_path().c_str(), filters.size(), filters.data(), "Font", false);
				if(!fileDialogResult)
					return;

				const std::filesystem::path path = std::filesystem::relative(fileDialogResult, Project::get_root_dir());

				std::shared_ptr<Font> font = Project::get_resource_manager().add_resource<Font>(path);
				if(!font)
					return;

				label.p_font = font;
			}
		});

		draw_component<LuaScriptComponent>("Lua Script", [](LuaScriptComponent &script) {
			if(!script.inited) {
				if(ImGui::Button("Choose script")) {
					static constexpr std::array<const char *, 1> filters = {"*.lua"};
					const char *fileDialogResult = tinyfd_openFileDialog("Select lua script", Project::get_absolute_path().c_str(), filters.size(), filters.data(), "Lua script", false);
					if(!fileDialogResult)
						return;

					script.path = std::filesystem::relative(fileDialogResult, Project::get_root_dir()).string();
					LuaScriptingModule::get_instance().init_script(script);
				}

				return;
			}

			ImGui::Text("Script: %s", script.path.string().c_str());

			if(ImGui::CollapsingHeader("Properties")) {
				script.self.for_each([](const sol::object &key, const sol::object &value) {
					switch(value.get_type()) {
					case sol::type::string:
						ImGui::LabelText(key.as<const char *>(), "%s", value.as<const char *>());
						break;
					case sol::type::number:
						ImGui::LabelText(key.as<const char *>(), "%g", value.as<float>());
						break;

					case sol::type::boolean:
						ImGui::LabelText(key.as<const char *>(), "%i", value.as<bool>());
						break;

					default:
						break;
					}
				});
			}

			// TODO: reload all scripts on playmode enter!!!!
			if(ImGui::Button("Reload"))
				LuaScriptingModule::get_instance().init_script(script);
		});

		draw_component<NativeScriptComponent>("Native Script", [](NativeScriptComponent &script) { ImGui::Text("Loaded script: %s", script.scriptName.c_str()); });
	}

	void InspectorPanel::draw_z_index_selector(i8 &zIndex) {
		int zIndexInt = zIndex;
		const i8 step = 1;
		ImGui::InputScalar("Z Index", ImGuiDataType_S8, &zIndexInt, &step);
		zIndex = std::min(zIndexInt, 125);// NOTE: 127 is reserved by gizmos, and 126 by outlines! (TODO: we probably should render them seperately)
	}

	void InspectorPanel::draw_color_selector(Color &color) {
		fif::vec4 colorNormalized = normalize_color(color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorNormalized));
		color = denormalize_color(colorNormalized);
	}
}// namespace fifed
