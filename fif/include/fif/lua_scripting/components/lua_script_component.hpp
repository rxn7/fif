#pragma once

#include <string>

#include "sol/load_result.hpp"

namespace fif::lua_scripting {
	struct LuaScriptComponent {
		sol::function updateFunc;
		sol::function renderFunc;
	};
}// namespace fif::lua_scripting
