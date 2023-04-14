#pragma once

#include "fif/core/layers/layer.h"

struct GLFWwindow;

namespace fif::core {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void render() override;
	};
}
