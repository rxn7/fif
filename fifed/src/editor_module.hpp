#pragma once

#include "editor_scene.hpp"
#include "fif/core/module.hpp"

class EditorModule final : public fif::core::Module {
public:
	FIF_MODULE_NAME(Editor)

	EditorModule();
	virtual ~EditorModule();

	void on_start(fif::core::Application &app) override;

private:
	void on_render_im_gui();
	std::shared_ptr<EditorScene> mp_Scene;
};
