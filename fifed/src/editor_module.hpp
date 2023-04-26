#pragma once

#include "components/camera_controller_component.hpp"
#include "components/grid_renderer_component.hpp"
#include "editor_scene.hpp"
#include "fif/core/module.hpp"
#include "fif/gfx/frame_buffer.hpp"

class EditorModule final : public fif::core::Module {
public:
	FIF_MODULE_NAME(Editor)

	EditorModule();
	virtual ~EditorModule();

	void on_start(fif::core::Application &app) override;
	void on_render() override;
	void on_update() override;

private:
	void on_render_im_gui();

private:
	std::unique_ptr<fif::gfx::FrameBuffer> mp_FrameBuffer;
	std::shared_ptr<EditorScene> mp_Scene;
	GridRendererComponent *mp_GridComponent;
	CameraControllerComponent *mp_CameraControllerComponent;
	fif::core::Entity *mp_EditorEntity;
};
