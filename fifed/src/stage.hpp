#pragma once

#include <fif/core/event/event.hpp>

namespace fifed {
	class FifedModule;
	class Stage {
	public:
		// TODO: These definetely should use the Invokables but I don't want to deal with handling life time of the bound functions rn
		virtual void update() {
		}
		virtual void pre_render() {
		}
		virtual void render() {
		}
		virtual void render_imgui() {
		}
		virtual void on_event([[maybe_unused]] Event &event) {
		}

	protected:
		Stage(FifedModule &fifedModule);

	protected:
		FifedModule &m_FifedModule;
	};
}// namespace fifed
