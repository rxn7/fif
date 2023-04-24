#include "editor_scene.hpp"
#include "ecs/entity.hpp"

void EditorScene::for_each(const std::function<void(fif::core::Entity &ent)> &func) {
	for_each(func, false);
}

void EditorScene::for_each(const std::function<void(fif::core::Entity &ent)> &func, bool excludeEditorEntites) {
	if(!excludeEditorEntites)
		for(fif::core::Entity &ent : m_EditorEntites)
			func(ent);

	Scene::for_each(func);
}
