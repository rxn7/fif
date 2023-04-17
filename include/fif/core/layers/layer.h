#pragma once

namespace fif::core {
	class Layer {
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void update([[maybe_unused]] float dt) = 0;
		virtual void renderImGui() = 0;
		virtual void render() = 0;
		virtual constexpr int getZIndex() { return 0; }
	};
}
