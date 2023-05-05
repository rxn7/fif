#pragma once

#include "sol/load_result.hpp"

namespace fif::lua_scripting {
	struct LuaScriptComponent {
		char path[1024] = "";
		bool loaded = false;
		sol::function updateFunc;
		sol::function renderFunc;
	};
}// namespace fif::lua_scripting
