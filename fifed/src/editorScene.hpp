#pragma once

#include "fif/core/ecs/scene.hpp"

class EditorScene : public fif::core::Scene {
public:
	void forEach(const std::function<void(fif::core::Entity &ent)> &func) override final;
	void forEach(const std::function<void(fif::core::Entity &ent)> &func, bool excludeEditorEntites = false);
	inline fif::core::Entity *createEditorEntity(const std::string &name) { return &m_EditorEntites.emplace_back(name); }

private:
	std::vector<fif::core::Entity> m_EditorEntites;
};
