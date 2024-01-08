#include "inspector_panel.hpp"

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
	InspectorPanel::InspectorPanel(Editor &editor, Scene &scene) : EditorPanel(editor), m_SelectedEntity(scene, entt::null) {
	}

	void InspectorPanel::on_render() {
		if(m_SelectedEntity.m_ID == entt::null)
			return;

		Scene &scene = Application::get_instance().get_scene();

		if(ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if(ImGui::BeginPopup("AddComponent")) {
			draw_add_component_entry<TagComponent>("Tag");
			draw_add_component_entry<TransformComponent>("Transform");
			draw_add_component_entry<SpriteComponent>("Sprite");
			draw_add_component_entry<QuadComponent>("Quad");
			draw_add_component_entry<CircleComponent>("Circle");
			draw_add_component_entry<LabelComponent>("Label");
			draw_add_component_entry<LuaScriptComponent>("LuaScript", nullptr, m_SelectedEntity);
			ImGui::EndPopup();
		}

		ImGui::Spacing();

		if(TagComponent *tag = m_SelectedEntity.try_get_component<TagComponent>()) {
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
			draw_color_selector(quad.tint);
			ImGui::DragFloat2("Size", glm::value_ptr(quad.size));
		});

		draw_component<SpriteComponent>("Sprite", [](SpriteComponent &sprite) {
			draw_color_selector(sprite.tint);
			ImGui::DragFloat2("Size", glm::value_ptr(sprite.size), 1.0f, 0.0f, std::numeric_limits<float>::max());

			if(sprite.p_texture) {
				ImGui::Text("Texture");
				ImGui::Image(reinterpret_cast<ImTextureID>(sprite.p_texture->get_id()), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
			} else
				ImGui::Text("Texture is not loaded!");

			if(ImGui::Button("Load texture")) {
				static constexpr std::array<const char *, 3> filters = {"*.png", "*.jpg", "*.gif"};

				const char *fileDialogResult = tinyfd_openFileDialog("Select tetxure", Project::get_absolute_path().c_str(), filters.size(), filters.data(), "Texture", false);
				if(!fileDialogResult)
					return;

				const std::filesystem::path path = std::filesystem::relative(fileDialogResult, Project::get_root_dir());

				std::shared_ptr<Texture> texture = Project::get_resource_manager().add_resource<Texture>(path);
				if(!texture)
					return;

				sprite.p_texture = texture;
				sprite.size = texture->get_size();
			}
		});

		draw_component<CircleComponent>("Circle", [](CircleComponent &circle) {
			draw_color_selector(circle.tint);
			ImGui::DragFloat("Radius", &circle.radius, 1.0f, 0.0f, std::numeric_limits<float>::max());
		});

		draw_component<LabelComponent>("Label", [&](LabelComponent &label) {
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

			ImGui::Text("Font: ");
			ImGui::Image(reinterpret_cast<ImTextureID>(Font::get_default()->get_texture()->get_id()), {256, 256});
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

			ImGui::Text("Script: %s", script.path.c_str());

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

	void InspectorPanel::draw_color_selector(Color &color) {
		fif::vec4 colorNormalized = normalize_color(color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorNormalized));
		color = denormalize_color(colorNormalized);
	}
}// namespace fifed
