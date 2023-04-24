#pragma once

#include "editorScene.hpp"
#include "fif/core/module.hpp"

class EditorModule final : public fif::core::Module {
public:
	FIF_MODULE_NAME(Editor)

	EditorModule();
	virtual ~EditorModule();

	void onStart(fif::core::Application &app) override;

private:
	void onRenderImGui();
	std::shared_ptr<EditorScene> mp_Scene;
};
