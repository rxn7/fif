#pragma once

#include "miniaudio.h"

namespace fif::audio {
	class AudioModule final : public core::Module {
	public:
		FIF_MODULE(AudioModule)
		AudioModule();
		~AudioModule();
		void on_start() override;

	private:
		ma_device m_Device;
		bool m_DeviceInitialized = false;
	};
}// namespace fif::audio
