#pragma once

#include "stage.hpp"
namespace fifed {
	class FifedModule;

	class ProjectManager final : public Stage {
	public:
		ProjectManager(FifedModule &editorModule);

		void update() override;
		void render_imgui() override;

		bool create_project_dialog();
		bool open_project_dialog();
		bool open_project(const std::filesystem::path &path);

	private:
		bool m_ProjectLoaded = false;
	};
}// namespace fifed
