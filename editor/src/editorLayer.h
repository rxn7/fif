#pragma once

#include "fif/core/event/event.h"
#include "fif/core/layers/layer.h"

#include <cinttypes>

class EditorLayer final : public fif::core::Layer {
public:
public:
	EditorLayer();
	virtual ~EditorLayer();

	void update(float dt) override;
	void render() override;
	void renderImGui() override;
	void onEvent(fif::core::Event &event) override;
	constexpr std::uint8_t getZIndex() const override { return 0; }

private:
	std::int32_t m_CircleSegments = 16;
};
