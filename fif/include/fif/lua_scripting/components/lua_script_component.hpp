#pragma once

#include "sol/sol.hpp"

namespace fif::lua_scripting {
	struct LuaScriptComponent final {
		sol::table self;

		struct {
			sol::function update;
			sol::function render;
		} hooks;

		std::string path;
	};
}// namespace fif::lua_scripting
