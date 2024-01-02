#include "fif/core/resource.hpp"
#include "fif/core/project.hpp"

namespace fif::core {
	Resource::Resource(const bool isEditorResource, const std::filesystem::path &path) : m_IsEditorResource(isEditorResource), m_Path(path) {
		if(m_IsEditorResource) {
			m_WorkingDirectoryRelativePath = path;
			return;
		}

		m_WorkingDirectoryRelativePath = Project::get_resource_path(path);
	}
}// namespace fif::core
