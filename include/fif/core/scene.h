#pragma once

#include "entt/entt.hpp"

namespace fif::core {
	class Scene {
	public:
		Scene();

	private:
		entt::registry m_Registry;
	};
}
