#include "inspector_panel.hpp"

#include "components/lua_script_component.hpp"
#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"
#include "fif/native_scripting/components/native_script_component.hpp"

#include <filesystem>
#include <imgui.h>
#include <tinyfiledialogs.h>

namespace fifed {
	InspectorPanel::InspectorPanel(Scene &scene) : m_SelectedEntity(scene, entt::null) {}

	template<typename T, typename... Args> static void draw_add_component_entry(const std::string &name, Entity &ent, Args &&...args) {
		if(ent.has_component<T>())
			return;

		if(ImGui::MenuItem(name.c_str())) {
			ent.add_component<T>(args...);
			ImGui::CloseCurrentPopup();
		}
	}

	template<typename T> static void draw_component(const std::string &name, Entity &ent, std::function<void(T &)> drawFunc) {
		T *comp = ent.try_get_component<T>();
		if(!comp)
			return;

		ImGui::PushID(typeid(T).hash_code() + static_cast<u32>(ent.m_ID));

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
			ent.remove_component<T>();

		ImGui::Separator();

		ImGui::PopID();
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
			// TODO: Automate this?
			draw_add_component_entry<TagComponent>("Tag", m_SelectedEntity);
			draw_add_component_entry<TransformComponent>("Transform", m_SelectedEntity);
			draw_add_component_entry<SpriteComponent>("Sprite", m_SelectedEntity);
			draw_add_component_entry<QuadComponent>("Quad", m_SelectedEntity);
			draw_add_component_entry<CircleComponent>("Circle", m_SelectedEntity);

			if(!m_SelectedEntity.has_component<LuaScriptComponent>()) {
				if(ImGui::MenuItem("Lua Script")) {
					const char *filter = "*.lua";
					char *path = tinyfd_openFileDialog("Select lua script", workingDirectoryStr.c_str(), 1, &filter, "Lua script", false);

					if(path)
						LuaScriptingModule::get_instance()->attach_script(m_SelectedEntity, path);

					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		ImGui::Spacing();

		if(TagComponent *tag = m_SelectedEntity.try_get_component<TagComponent>()) {
			std::strncpy(m_TagBuffer.data(), tag->tag.c_str(), tag->tag.size() + 1);
			if(ImGui::InputText("Tag", m_TagBuffer.data(), m_TagBuffer.size()))
				tag->tag = m_TagBuffer.data();
		}

		draw_component<TransformComponent>("Transform", m_SelectedEntity, [](TransformComponent &transform) {
			ImGui::DragFloat2("Position", glm::value_ptr(transform.position));
			ImGui::DragFloat2("Scale", glm::value_ptr(transform.scale), 0.1f);

			float angleDegrees = glm::degrees(transform.angleRadians);
			ImGui::DragFloat("Angle", &angleDegrees, 1.0f);
			transform.angleRadians = glm::radians(angleDegrees);
		});

		draw_component<QuadComponent>("Quad", m_SelectedEntity, [](QuadComponent &quad) {
			draw_color_selector(quad.tint);
			ImGui::DragFloat2("Size", glm::value_ptr(quad.size));
		});

		draw_component<SpriteComponent>("Sprite", m_SelectedEntity, [&workingDirectoryStr](SpriteComponent &sprite) {
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

		draw_component<CircleComponent>("Circle", m_SelectedEntity, [](CircleComponent &circle) {
			draw_color_selector(circle.tint);
			ImGui::DragFloat("Radius", &circle.radius, 1.0f, 0.0f, std::numeric_limits<float>::max());
		});

		draw_component<LuaScriptComponent>("Lua Script", m_SelectedEntity, [](LuaScriptComponent &script) {
			ImGui::Text("Script: %s", script.filepath.stem().c_str());

			if(ImGui::Button("Reload"))
				LuaScriptingModule::get_instance()->init_script(script);

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
		});

		draw_component<NativeScriptComponent>("Native Script", m_SelectedEntity, [](NativeScriptComponent &script) { ImGui::Text("Loaded script: %s", script.scriptName.c_str()); });
	}

	void InspectorPanel::draw_color_selector(Color &color) {
		fif::vec4 colorNormalized = normalize_color(color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorNormalized));
		color = denormalize_color(colorNormalized);
	}
}// namespace fifed
