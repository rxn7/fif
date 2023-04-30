#include "fifed.hpp"
#include "common.hpp"
#include "editor_module.hpp"

#include "fif/fif.hpp"

const WindowProperties WINDOW_PROPS = {
	.title = "fifed",
	.size = glm::i16vec2(1280, 720),
	.vsync = false,
};

Fifed::Fifed() : Application(WINDOW_PROPS) {
	attach_module<InputModule>();
	attach_module<GfxModule>();
	attach_module<EditorModule>();
	attach_module<ImGuiModule>();
	attach_module<LuaScriptingModule>();
}

Application *get_application() {
	return new Fifed();
}
