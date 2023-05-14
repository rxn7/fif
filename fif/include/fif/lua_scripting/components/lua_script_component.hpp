#pragma once

#include "sol/sol.hpp"

namespace fif::lua_scripting {
	struct LuaScriptComponent final {
		std::string path;

		bool loaded = false;
		sol::state luaState;
		sol::function updateFunc;
		sol::function renderFunc;
	};
}// namespace fif::lua_scripting
