#pragma once

#include <fif/core/invokable.hpp>
#include <fif/gfx/frame_buffer.hpp>

namespace fif_runtime {
	class RuntimeModule : public Module {
	public:
		FIF_MODULE(RuntimeModule);

		RuntimeModule();
		~RuntimeModule();

	private:
		void on_start();
		void on_update();
		void on_event(Event &event);

	private:
		FrameBuffer m_FrameBuffer;
		Callback<> m_StartCallback;
		Callback<> m_UpdateCallback;
		Callback<Event &> m_EventCallback;
	};
}// namespace fif_runtime
