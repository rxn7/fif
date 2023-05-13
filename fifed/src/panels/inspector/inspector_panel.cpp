#include "inspector_panel.hpp"

#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "fif/native_scripting/components/native_script_component.hpp"

#include "imgui.h"
#include "tinyfiledialogs.h"
#include <filesystem>

namespace fifed {
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

		ImGui::Separator();

		ImGui::PopID();
	}

	void InspectorPanel::on_render() {
		if(m_SelectedEntity == entt::null)
			return;

		Scene &scene = Application::get_instance()->get_scene();

		std::stringstream workingDirectorySs;
		workingDirectorySs << std::filesystem::current_path() << "/";
		const std::string workingDirectoryStr = workingDirectorySs.str();

		if(ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if(ImGui::BeginPopup("AddComponent")) {
			// TODO: Automate this?
			draw_add_component_entry<TagComponent>("Tag", m_SelectedEntity, scene);
			draw_add_component_entry<TransformComponent>("Transform", m_SelectedEntity, scene);
			draw_add_component_entry<SpriteComponent>("Sprite", m_SelectedEntity, scene);
			draw_add_component_entry<QuadComponent>("Quad", m_SelectedEntity, scene);
			draw_add_component_entry<CircleComponent>("Circle", m_SelectedEntity, scene);
			draw_add_component_entry<LuaScriptComponent>("Lua Script", m_SelectedEntity, scene);
			ImGui::EndPopup();
		}

		ImGui::Spacing();

		if(TagComponent *tag = scene.get_registry().try_get<TagComponent>(m_SelectedEntity)) {
			std::strncpy(m_TagBuffer.data(), tag->tag.c_str(), tag->tag.size());
			if(ImGui::InputText("Tag", m_TagBuffer.data(), m_TagBuffer.size()))
				tag->tag = m_TagBuffer.data();
		}

		draw_component<TransformComponent>("Transform", m_SelectedEntity, scene, [](TransformComponent &transform) {
			ImGui::DragFloat2("Position", glm::value_ptr(transform.position));
			ImGui::DragFloat2("Scale", glm::value_ptr(transform.scale), 0.1f);

			float angleDegrees = glm::degrees(transform.angleRadians);
			ImGui::DragFloat("Angle", &angleDegrees, 1.0f, -360.0f, 360.0f);
			transform.angleRadians = glm::radians(angleDegrees);
		});

		draw_component<QuadComponent>("Quad", m_SelectedEntity, scene, [](QuadComponent &quad) {
			draw_color_selector(quad.tint);
			ImGui::DragFloat2("Size", glm::value_ptr(quad.size));
		});

		draw_component<SpriteComponent>("Sprite", m_SelectedEntity, scene, [&workingDirectoryStr](SpriteComponent &sprite) {
			draw_color_selector(sprite.tint);
			ImGui::DragFloat2("Size", glm::value_ptr(sprite.size), 1.0f, 0.0f, std::numeric_limits<float>::max());

			if(sprite.p_texture) {
				ImGui::Text("Texture");
				ImGui::Image(reinterpret_cast<ImTextureID>(sprite.p_texture->get_id()), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
			} else
				ImGui::Text("Texture is not loaded!");

			if(ImGui::Button("Load texture")) {
				constexpr std::array<const char *, 2> filterPatterns = {"*.png", "*.jpg"};

				char *path = tinyfd_openFileDialog("Select texture", workingDirectoryStr.c_str(), filterPatterns.size(), filterPatterns.data(), "Image", false);

				if(path)
					sprite.set_texture(Texture::load(path, GL_NEAREST));
			}
		});

		draw_component<CircleComponent>("Circle", m_SelectedEntity, scene, [](CircleComponent &circle) {
			draw_color_selector(circle.tint);
			ImGui::DragFloat("Radius", &circle.radius, 1.0f, 0.0f, std::numeric_limits<float>::max());
		});

		draw_component<LuaScriptComponent>("Lua Script", m_SelectedEntity, scene, [&workingDirectoryStr](LuaScriptComponent &script) {
			if(script.loaded)
				ImGui::Text("Script: %s", script.path.c_str());
			else
				ImGui::Text("No script is attached to this component!");

			if(ImGui::Button("Load script")) {
				constexpr const char *filterPattern = "*.lua";
				char *path = tinyfd_openFileDialog("Select lua script", workingDirectoryStr.c_str(), 0, &filterPattern, "Lua script", false);

				if(path)
					LuaScriptingModule::get_instance()->attach_script(script, path);
			}
		});

		draw_component<NativeScriptComponent>("Native Script", m_SelectedEntity, scene,
											  [](NativeScriptComponent &script) { ImGui::Text("Loaded script: %s", script.scriptName.c_str()); });
	}

	void InspectorPanel::draw_color_selector(Color &color) {
		glm::vec4 colorNormalized = get_normalized_color(color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorNormalized));
		color = get_color_from_normalized(colorNormalized);
	}
}// namespace fifed
