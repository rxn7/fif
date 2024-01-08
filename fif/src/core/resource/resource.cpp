#include "fif/core/resource/resource.hpp"
#include "fif/core/project.hpp"

namespace fif::core {
	Resource::Resource(const std::filesystem::path &path) : m_Path(path) {
	}

	std::filesystem::path Resource::get_path_relative() const {
		if(m_IsEditorResource)
			return m_Path;

		return Project::get_resource_path(m_Path);
	}
}// namespace fif::core
