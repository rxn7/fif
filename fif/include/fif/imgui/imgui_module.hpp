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
		inline Invokable<> &get_render_hook() {
			return m_RenderHook;
		}

	protected:
		void on_start() override;
		void on_render() override;
		void on_event(core::Event &event) override;

	private:
		void apply_default_theme() const;

	private:
		Invokable<> m_RenderHook;
	};
}// namespace fif::imgui
