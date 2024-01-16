#include "resource_browser.hpp"
#include "fifed_module.hpp"

#include <fif/core/project.hpp>
#include <fif/core/serialization/scene_serializer.hpp>

#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <imgui_internal.h>
#include <tinyfiledialogs.h>

namespace fifed {
	ResourceBrowserPanel::ResourceBrowserPanel(Editor &editor) : EditorPanel(editor), m_RootDirectory(Project::get_root_dir()), m_CurrentDirectory(m_RootDirectory) {}

	void ResourceBrowserPanel::on_render() {
		const bool isInRoot = m_CurrentDirectory == m_RootDirectory;

		if(!isInRoot) {
			const std::string relativePath = std::filesystem::relative(m_CurrentDirectory, m_RootDirectory).string();
			if(ImGui::ArrowButton(relativePath.c_str(), ImGuiDir_Left)) {
				current_directory_go_back();
			}
		}

		bool fileContextPopupOpen = false;

		// TODO: Use clipper to iterate over only visible entries
		// TODO: Call filesystem api on interval or when something changes in the directory.
		u32 entryIdx = 0;
		for(const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			const std::string entryFileName = entry.path().filename().string();
			ImGui::PushID((entryFileName + "##resourceBrowserEntry").c_str());

			const vec2 size{32, 32};

			if(entry.is_directory()) {
				bool pressed = FifedModule::get_instance().get_icon_manager().imgui_button(entryFileName.c_str(), IconType::DIRECTORY, size);
				ImGui::SameLine(0.0f, 0.0f);

				pressed |= ImGui::Button((entryFileName + "##btn").c_str(), {0, size.y});

				if(pressed) {
					m_CurrentDirectory = entry.path();
				}
			} else {
				bool pressed = FifedModule::get_instance().get_icon_manager().imgui_button(entryFileName.c_str(), IconType::FILE, size);
				ImGui::SameLine(0.0f, 0.0f);

				pressed |= ImGui::ButtonEx((entryFileName + "##btn").c_str(), {0, size.y}, ImGuiButtonFlags_PressedOnDoubleClick);

				if(pressed) {
					on_file_double_click(entry.path());
				}
			}

			if(ImGui::BeginPopupContextItem(("ResourceBrowserFileContextMenu##" + std::to_string(entryIdx)).c_str())) {
				fileContextPopupOpen = true;
				render_file_context_menu(entry.path());
				ImGui::EndPopup();
			}

			ImGui::PopID();

			++entryIdx;
		}

		if(!fileContextPopupOpen && ImGui::BeginPopupContextWindow("ResourceBrowserContextMenu")) {
			render_window_context_menu();
			ImGui::EndPopup();
		}
	}

	void ResourceBrowserPanel::render_window_context_menu() {
		if(ImGui::SmallButton("Create new scene")) {
			const char *filter = "*.fifscene";
			if(const char *path = tinyfd_saveFileDialog("Create new scene", (m_CurrentDirectory / "scene.fifscene").c_str(), 1, &filter, "Fif Scene")) {
				std::ofstream fileStream(path);
			}
			ImGui::CloseCurrentPopup();
		}

		if(ImGui::SmallButton("Create new lua script")) {
			const char *filter = "*.lua";
			if(const char *path = tinyfd_saveFileDialog("Create new lua script", (m_CurrentDirectory / "script.lua").c_str(), 1, &filter, "Lua Script")) {
				std::ofstream fileStream(path);
			}
			ImGui::CloseCurrentPopup();
		}
	}

	void ResourceBrowserPanel::render_file_context_menu(const std::filesystem::path &path) {
		const std::filesystem::path relativePath = std::filesystem::relative(path, Project::get_absolute_path());
		ImGui::Text("%s", relativePath.c_str());

		if(ImGui::SmallButton("Delete")) {
			if(tinyfd_messageBox("Are you sure?", ("Do you want to delete " + relativePath.string() + "?").c_str(), "yesno", "question", 0)) {
				std::filesystem::remove(path);
			}
			ImGui::CloseCurrentPopup();
		}

		// TODO: Update resources that use this path!!!
		if(ImGui::SmallButton("Rename")) {
			const std::string filename = relativePath.filename().string();
			if(const char *newName = tinyfd_inputBox("Rename file", ("Rename file " + filename).c_str(), filename.c_str())) {
				Logger::debug("Renamed file '%s' to '%s'", relativePath.c_str(), newName);
				std::filesystem::rename(path, path.parent_path() / newName);
			}
			ImGui::CloseCurrentPopup();
		}
	}

	void ResourceBrowserPanel::on_file_double_click(const std::filesystem::path &path) {
		Logger::debug("Double clicked file with extension: %s", path.extension().c_str());

		if(path.extension() == SceneSerializer::get_file_extension()) {
			m_Editor.open_scene(Project::get_resource_path(path));
		}
	}

	void ResourceBrowserPanel::current_directory_go_back() { m_CurrentDirectory = m_CurrentDirectory.parent_path(); }
}// namespace fifed
