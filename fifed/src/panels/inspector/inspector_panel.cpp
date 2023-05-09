#include "inspector_panel.hpp"
#include "components/circle_component.hpp"
#include "components/quad_component.hpp"
#include "components/sprite_component.hpp"
#include "components/transform_component.hpp"
#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/native_scripting/components/native_script_component.hpp"
#include "imgui.h"

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

		ImGui::PopID();
	}

	void InspectorPanel::on_render() {
		if(m_SelectedEntity == entt::null)
			return;

		Scene &scene = Application::get_instance()->get_scene();

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

		ImGui::Separator();

		if(TagComponent *tag = scene.get_registry().try_get<TagComponent>(m_SelectedEntity)) {
			std::strncpy(m_TagBuffer.data(), tag->tag.c_str(), tag->tag.size());
			if(ImGui::InputText("Tag", m_TagBuffer.data(), m_TagBuffer.size()))
				tag->tag = m_TagBuffer.data();
		}

		draw_component<TransformComponent>("Transform", m_SelectedEntity, scene, [](TransformComponent &transform) {
			ImGui::DragFloat2("Position", glm::value_ptr(transform.position));

			ImGui::Separator();

			float angleDegrees = glm::degrees(transform.angle);
			ImGui::DragFloat("Angle", &angleDegrees, 1.0f, 0.0f, 360.0f);
			transform.angle = glm::radians(angleDegrees);
		});

		draw_component<QuadComponent>("Quad", m_SelectedEntity, scene, [](QuadComponent &quad) {
			draw_color_selector(quad.tint);
			ImGui::Separator();
			ImGui::DragFloat2("Size", glm::value_ptr(quad.size));
		});

		draw_component<SpriteComponent>("Sprite", m_SelectedEntity, scene, [](SpriteComponent &sprite) {
			draw_color_selector(sprite.tint);
			ImGui::Separator();
			ImGui::DragFloat2("Size", glm::value_ptr(sprite.size), 1.0f, 0.0f, std::numeric_limits<float>::max());

			ImGui::Separator();

			if(sprite.p_texture) {
				ImGui::Text("Texture");
				ImGui::Image(reinterpret_cast<ImTextureID>(sprite.p_texture->get_id()), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
			} else
				ImGui::Text("Texture is not loaded!");

			ImGui::Separator();

			if(ImGui::Button("Load texture"))
				ImGui::OpenPopup("LoadTexture");

			if(ImGui::BeginPopup("LoadTexture")) {
				ImGui::InputText("File path", sprite.path, 1024);
				if(ImGui::Button("Load")) {
					sprite.p_texture = std::make_shared<Texture>(sprite.path, GL_NEAREST);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		});

		draw_component<CircleComponent>("Circle", m_SelectedEntity, scene, [](CircleComponent &circle) {
			draw_color_selector(circle.tint);
			ImGui::DragFloat("Radius", &circle.radius, 1.0f, 0.0f, std::numeric_limits<float>::max());

			ImGui::Separator();

			bool useFrag = circle.segments == 0;
			if(ImGui::Checkbox("Use frag", &useFrag))
				circle.segments = useFrag ? 0 : 16;

			if(!useFrag) {
				static constexpr u16 MIN_SEGMENTS = 5;
				static constexpr u16 MAX_SEGMENTS = 100;
				ImGui::DragScalar("Segments", ImGuiDataType_U16, &circle.segments, 1, &MIN_SEGMENTS, &MAX_SEGMENTS);
			}
		});

		draw_component<LuaScriptComponent>("Lua Script", m_SelectedEntity, scene, [](LuaScriptComponent &script) {
			if(script.loaded)
				ImGui::Text("Script: %s", script.path);
			else
				ImGui::Text("No script is attached to this component!");

			ImGui::Separator();

			if(ImGui::Button("Load script"))
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

		draw_component<NativeScriptComponent>("Native Script", m_SelectedEntity, scene,
											  [](NativeScriptComponent &script) { ImGui::Text("Loaded script: %s", script.scriptName.c_str()); });
	}

	void InspectorPanel::draw_color_selector(Color &color) {
		glm::vec4 colorNormalized = get_normalized_color(color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorNormalized));
		color = get_color_from_normalized(colorNormalized);
	}
}// namespace fifed