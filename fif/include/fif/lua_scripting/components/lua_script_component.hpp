#pragma once

#include "sol/sol.hpp"

namespace fif::lua_scripting {
	struct LuaScriptComponent final {
		sol::table self;
		struct {
			sol::function update;
			sol::function render;
		} hooks;

		std::filesystem::path filepath;
		bool inited = false;
	};
}// namespace fif::lua_scripting
