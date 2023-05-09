#include "fifed.hpp"
#include "editor_module.hpp"

#include "fif/fif.hpp"
#include "fif/native_scripting/native_scripting_module.hpp"

const ApplicationProperties APP_PROPS = {
	.windowProps =
		{
			.title = "Fif Editor",
			.size = glm::i16vec2(1280, 720),
			.iconPath = "assets/textures/logo.png",
			.vsync = false,
		},

	.createDefaultScene = true,
};

namespace fifed {
	Fifed::Fifed() : Application(APP_PROPS) {
		attach_module<InputModule>();
		attach_module<GfxModule>();
		attach_module<ImGuiModule>();
		attach_module<EditorModule>();
		attach_module<LuaScriptingModule>();
		attach_module<NativeScriptingModule>();
	}
}// namespace fifed

Application *get_application() {
	return reinterpret_cast<Application *>(new fifed::Fifed());
}
