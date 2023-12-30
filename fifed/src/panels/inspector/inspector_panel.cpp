#include "inspector_panel.hpp"

#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/label_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/gfx/text/font.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"
#include "fif/native_scripting/components/native_script_component.hpp"

#include <filesystem>
#include <imgui.h>
#include <optional>
#include <tinyfiledialogs.h>

namespace fifed {
	InspectorPanel::InspectorPanel(Scene &scene) : m_SelectedEntity(scene, entt::null) {
	}

	void InspectorPanel::on_render() {
		if(m_SelectedEntity.m_ID == entt::null)
			return;

		Scene &scene = Application::get_instance()->get_scene();

		std::stringstream workingDirectorySs;
		workingDirectorySs << std::filesystem::current_path() << "/";
		const std::string workingDirectoryStr = workingDirectorySs.str();

		if(ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if(ImGui::BeginPopup("AddComponent")) {
			draw_add_component_entry<TagComponent>("Tag");
			draw_add_component_entry<TransformComponent>("Transform");
			draw_add_component_entry<SpriteComponent>("Sprite");
			draw_add_component_entry<QuadComponent>("Quad");
			draw_add_component_entry<CircleComponent>("Circle");
			draw_add_component_entry<LabelComponent>("Label");
			draw_add_component_entry<LuaScriptComponent>(
				"Lua Script",
				[&](auto &script) {
					const char *filter = "*.lua";
					char *path = tinyfd_openFileDialog("Select lua script", workingDirectoryStr.c_str(), 1, &filter, "Lua script", false);

					if(path) {
						script.filepath = path;
						LuaScriptingModule::get_instance()->init_script(script);
					}
				},
				m_SelectedEntity);

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

		draw_component<SpriteComponent>("Sprite", [&workingDirectoryStr](SpriteComponent &sprite) {
			draw_color_selector(sprite.tint);
			ImGui::DragFloat2("Size", glm::value_ptr(sprite.size), 1.0f, 0.0f, std::numeric_limits<float>::max());

			if(sprite.p_texture) {
				ImGui::Text("Texture");
				ImGui::Image(reinterpret_cast<ImTextureID>(sprite.p_texture->get_id()), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
			} else
				ImGui::Text("Texture is not loaded!");

			if(ImGui::Button("Load texture")) {
				constexpr std::array<const char *, 3> filterPatterns = {"*.png", "*.jpg", "*.gif"};
				char *path = tinyfd_openFileDialog("Select texture", workingDirectoryStr.c_str(), filterPatterns.size(), filterPatterns.data(), "Image", false);

				if(path)
					sprite.set_texture(std::make_shared<Texture>(path, GL_NEAREST));
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

			ImGui::DragFloat("Size", &label.fontSize, 1.0f, 0.0f, std::numeric_limits<float>::max());
			ImGui::DragFloat("Character Spacing Factor", &label.charSpacingFactor, 0.05f, 0.0f, std::numeric_limits<float>::max());
			ImGui::DragFloat("Line Height Factor", &label.lineHeightFactor, 0.05f, 0.0f, std::numeric_limits<float>::max());
			draw_color_selector(label.color);

			{
				static const char *items[] = {"Left", "Center", "Right"};
				ImGui::Combo("Horizontal Align", (int *)&label.horizontalAlign, items, IM_ARRAYSIZE(items));
			}

			{
				static const char *items[] = {"Top", "Center", "Bottom"};
				ImGui::Combo("Vertical Align", (int *)&label.verticalAlign, items, IM_ARRAYSIZE(items));
			}

			const vec2 textSize = label.calculate_text_size();
			ImGui::LabelText("Size", "%f, %f", textSize.x, textSize.y);
		});

		draw_component<LuaScriptComponent>("Lua Script", [](LuaScriptComponent &script) {
			ImGui::Text("Script: %s", script.filepath.stem().c_str());

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

			if(ImGui::Button("Reload"))
				LuaScriptingModule::get_instance()->init_script(script);
		});

		draw_component<NativeScriptComponent>("Native Script", [](NativeScriptComponent &script) { ImGui::Text("Loaded script: %s", script.scriptName.c_str()); });
	}

	void InspectorPanel::draw_color_selector(Color &color) {
		fif::vec4 colorNormalized = normalize_color(color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorNormalized));
		color = denormalize_color(colorNormalized);
	}
}// namespace fifed
