#pragma once

#include "fif/core/ecs/entity.hpp"

#include <sol/sol.hpp>

namespace fif::lua_scripting {
	struct LuaScriptComponent final {
		LuaScriptComponent(const core::Entity &ent, const std::string &path = "") : path(path), entity(ent) {}

		sol::table self;

		sol::function start_hook;
		sol::function update_hook;
		sol::function render_hook;

		std::string path = "";
		bool inited = false;
		bool firstFrame = true;
		core::Entity entity;
	};
}// namespace fif::lua_scripting
