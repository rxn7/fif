#include "lua_register.hpp"

#include "fif/input/input_module.hpp"

#ifdef FIF_LUA_SCRIPTING
#include "fif/lua_scripting/lua_scripting_module.hpp"
#endif

namespace fif::input {
#ifdef FIF_LUA_SCRIPTING
	void register_lua_types(InputModule &inputModule) {
		sol::table inputTable = lua_scripting::LuaScriptingModule::get_instance().m_Lua.create_named_table("Input");
		inputTable.set_function("is_key_held", [&inputModule](const i32 key) { return inputModule.is_key_held(key); });
		inputTable.set_function("is_key_just_pressed", [&inputModule](const i32 key) { return inputModule.is_key_just_pressed(key); });
		inputTable.set_function("get_mouse_position", [&inputModule]() { return inputModule.get_mouse_position(); });

		// clang-format off
		inputTable.new_enum<i32>("Key", {
			{"Space", GLFW_KEY_SPACE},
			{"Apostrophe", GLFW_KEY_APOSTROPHE},
			{"Comma", GLFW_KEY_COMMA},
			{"Minus", GLFW_KEY_MINUS},
			{"Period", GLFW_KEY_PERIOD},
			{"Slash", GLFW_KEY_SLASH},
			{"Alpha0", GLFW_KEY_0},
			{"Alpha1", GLFW_KEY_1},
			{"Alpha2", GLFW_KEY_2},
			{"Alpha3", GLFW_KEY_3},
			{"Alpha4", GLFW_KEY_4},
			{"Alpha5", GLFW_KEY_5},
			{"Alpha6", GLFW_KEY_6},
			{"Alpha7", GLFW_KEY_7},
			{"Alpha8", GLFW_KEY_8},
			{"Alpha9", GLFW_KEY_9},
			{"Semicolon", GLFW_KEY_SEMICOLON},
			{"Equal", GLFW_KEY_EQUAL},
			{"A", GLFW_KEY_A},
			{"B", GLFW_KEY_B},
			{"C", GLFW_KEY_C},
			{"D", GLFW_KEY_D},
			{"E", GLFW_KEY_E},
			{"F", GLFW_KEY_F},
			{"G", GLFW_KEY_G},
			{"H", GLFW_KEY_H},
			{"I", GLFW_KEY_I},
			{"J", GLFW_KEY_J},
			{"K", GLFW_KEY_K},
			{"L", GLFW_KEY_L},
			{"M", GLFW_KEY_M},
			{"N", GLFW_KEY_N},
			{"O", GLFW_KEY_O},
			{"P", GLFW_KEY_P},
			{"Q", GLFW_KEY_Q},
			{"R", GLFW_KEY_R},
			{"S", GLFW_KEY_S},
			{"T", GLFW_KEY_T},
			{"U", GLFW_KEY_U},
			{"V", GLFW_KEY_V},
			{"W", GLFW_KEY_W},
			{"X", GLFW_KEY_X},
			{"Y", GLFW_KEY_Y},
			{"Z", GLFW_KEY_Z},
			{"LeftBracket", GLFW_KEY_LEFT_BRACKET},
			{"Backslash", GLFW_KEY_BACKSLASH},
			{"RightBracket", GLFW_KEY_RIGHT_BRACKET}
		});
		// clang-format on
	}
#else
	void register_lua_types([[maybe_unused]] InputModule &inputModule) {}
#endif
}// namespace fif::input
