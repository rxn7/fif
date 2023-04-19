#pragma once

#include "fif/core/module.h"

class EditorModule final : public fif::core::Module {
  public:
	FIF_MODULE_NAME(Editor)

	EditorModule();
	virtual ~EditorModule();

	void onStart(fif::core::Application &app) override;

  private:
	void onRenderImGui();
};
