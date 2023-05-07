#include "fifed.hpp"
#include "common.hpp"
#include "editor_module.hpp"

#include "fif/fif.hpp"

const WindowProperties WINDOW_PROPS = {
	.title = "Fif Editor",
	.size = glm::i16vec2(1280, 720),
	.iconPath = "assets/textures/logo.png",
	.vsync = false,
};

namespace fifed {
	Fifed::Fifed() : Application(WINDOW_PROPS) {
		attach_module<InputModule>();
		attach_module<GfxModule>();
		attach_module<EditorModule>();
		attach_module<ImGuiModule>();
		attach_module<LuaScriptingModule>();
	}
}// namespace fifed

Application *get_application() {
	return reinterpret_cast<Application *>(new fifed::Fifed());
}
