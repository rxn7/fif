#include "editorScene.hpp"
#include "ecs/entity.hpp"

void EditorScene::forEach(const std::function<void(fif::core::Entity &ent)> &func) {
	forEach(func, false);
}

void EditorScene::forEach(const std::function<void(fif::core::Entity &ent)> &func, bool excludeEditorEntites) {
	if (!excludeEditorEntites) {
		for (fif::core::Entity &ent : m_EditorEntites) {
			func(ent);
		}
	}

	Scene::forEach(func);
}
