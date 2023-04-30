#pragma once

#include "common.hpp"

#include "fif/core/event/event.hpp"
#include "fif/core/module.hpp"
#include "fif/gfx/frame_buffer.hpp"

#include <memory>

class EditorModule final : public Module {
public:
	FIF_MODULE_NAME(Editor)

	EditorModule();
	virtual ~EditorModule();

	void on_start(Application &app) override;
	void on_render() override;
	void on_update() override;
	void on_event(Event &event) override;

private:
	void on_render_im_gui();

private:
	std::unique_ptr<FrameBuffer> mp_FrameBuffer;
};
