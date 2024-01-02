#pragma once

namespace fif::core {
	class Resource {
	public:
		inline const std::filesystem::path &get_working_directory_relative_path() const {
			return m_WorkingDirectoryRelativePath;
		}

		inline const std::filesystem::path &get_path() const {
			return m_Path;
		}

	protected:
		Resource() {
		}

		Resource(const bool isEditorResource, const std::filesystem::path &path);

	protected:
		bool m_IsEditorResource = true;
		std::filesystem::path m_WorkingDirectoryRelativePath;
		std::filesystem::path m_Path;
	};
}// namespace fif::core
