#pragma once

#include "fif/core/ecs/scene.hpp"

class EditorScene : public fif::core::Scene {
public:
	void for_each(const std::function<void(fif::core::Entity &ent)> &func) override final;
	void for_each(const std::function<void(fif::core::Entity &ent)> &func, bool excludeEditorEntites = false);

	inline fif::core::Entity *create_editor_entity() { return &m_EditorEntites.emplace_back(); }

private:
	std::vector<fif::core::Entity> m_EditorEntites;
};
