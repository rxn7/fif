#pragma once

#include "fif/core/project.hpp"

namespace fif::core {
	class ProjectSerializer final {
	public:
		ProjectSerializer(const std::shared_ptr<Project> &project);

		void serialize();
		bool deserialize();

	private:
		std::shared_ptr<Project> m_Project;
	};
}// namespace fif::core
