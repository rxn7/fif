#define MINIAUDIO_IMPLEMENTATION
#include "fif/audio/audio_module.hpp"

namespace fif::audio {
	FIF_MODULE_INSTANCE_IMPL(AudioModule)

	AudioModule::AudioModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	AudioModule::~AudioModule() {
		if(m_DeviceInitialized)
			ma_device_uninit(&m_Device);
	}

	void AudioModule::on_start() {
		ma_device_config cfg = ma_device_config_init(ma_device_type_playback);
		if(ma_device_init(NULL, &cfg, &m_Device) != MA_SUCCESS) {
			core::Logger::error("Failed to initialize the miniaudio device.");
			return;
		}

		m_DeviceInitialized = true;

		if(ma_device_start(&m_Device) != MA_SUCCESS) {
			core::Logger::error("Failed to start the miniaudio device.");
			ma_device_uninit(&m_Device);
			return;
		}
	}
}// namespace fif::audio
