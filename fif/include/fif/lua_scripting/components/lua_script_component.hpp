#pragma once

#include "sol/load_result.hpp"

namespace fif::lua_scripting {
	struct LuaScriptComponent final {
		std::string path;

		bool loaded = false;
		sol::function updateFunc;
		sol::function renderFunc;
	};
}// namespace fif::lua_scripting
