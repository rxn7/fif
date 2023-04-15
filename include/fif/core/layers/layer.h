#pragma once

namespace fif::core {
	class Layer {
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void render() {}
		virtual constexpr int getZIndex() { return 0; }
	};
}
