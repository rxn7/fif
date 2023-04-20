#pragma once

#include "fif/core/module.hpp"

class EditorModule final : public fif::core::Module {
  public:
	FIF_MODULE_NAME(Editor)

	EditorModule();
	virtual ~EditorModule();

	void onStart(fif::core::Application &app) override;

  private:
	static void onRenderImGui();
};
