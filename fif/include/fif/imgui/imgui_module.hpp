#pragma once

#include "fif/core/invokable.hpp"
#include "fif/core/module.hpp"

#define IM_VEC2_CLASS_EXTRA                                                                                                                                                                            \
	constexpr ImVec2(const glm::vec2 &f) : x(f.x), y(f.y) {                                                                                                                                            \
	}                                                                                                                                                                                                  \
	operator glm::vec2() const {                                                                                                                                                                       \
		return glm::vec2(x, y);                                                                                                                                                                        \
	}

#define IM_VEC4_CLASS_EXTRA                                                                                                                                                                            \
	constexpr ImVec4(const glm::vec4 &f) : x(f.x), y(f.y), z(f.z), w(f.w) {                                                                                                                            \
	}                                                                                                                                                                                                  \
	operator glm::vec4() const {                                                                                                                                                                       \
		return glm::vec4(x, y, z, w);                                                                                                                                                                  \
	}

#include <imgui.h>

namespace fif::imgui {
	typedef std::function<void(void)> ImGuiRenderFunc;

	class ImGuiModule final : public core::Module {
	public:
		FIF_MODULE(ImGuiModule)

		ImGuiModule();
		virtual ~ImGuiModule();

		bool begin_dockspace() const;

	public:
		Invokable<> m_RenderHook;

	private:
		void apply_default_theme() const;
		void on_start();
		void on_render();
		void on_event(core::Event &event);

	private:
		Callback<> m_StartCallback;
		Callback<> m_RenderCallback;
		Callback<core::Event &> m_EventCallback;
	};
}// namespace fif::imgui
