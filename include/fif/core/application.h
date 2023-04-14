#pragma once

#include "window.h"
#include <memory>

namespace fif::core {
	class Application {
	public:
		Application(const WindowProperties &windowProperties);
		virtual ~Application();

		inline const Window &getWindow() const { return *mp_Window; }
		void start();
		virtual void startFrame();
		virtual void endFrame();

	private:
		static Application *s_Instance;
		std::unique_ptr<Window> mp_Window;
	};
}
