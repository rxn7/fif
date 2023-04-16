#pragma once

#include "fif/core/layers/layer.h"

#include <cinttypes>

class EditorLayer : public fif::core::Layer {
public:
	void update(float dt) override;
	void render() override;
	void renderImGui() override;

private:
	std::int32_t m_CircleSegments = 16;
};
