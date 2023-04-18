#pragma once

#include "fif/core/module.h"

class EditorModule final : public fif::core::Module {
public:
	EditorModule();
	virtual ~EditorModule();

	constexpr std::string_view getName() const override { return "Editor"; }
	void onStart(fif::core::Application &app) override;

private:
	void onRenderImGui();
};
