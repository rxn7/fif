#pragma once

#include "fif/core/layers/layer.h"

class EditorLayer : public fif::core::Layer {
public:
	void update(float dt) override;
	void render() override;
	void renderImGui() override;
};
