#pragma once

#include "fif/core/ecs/entity.hpp"

#include <sol/sol.hpp>

namespace fif::lua_scripting {
	struct LuaScriptComponent final {
		LuaScriptComponent(const core::Entity &ent) : entity(ent) {}

		sol::table self;
		struct {
			sol::function start;
			sol::function update;
			sol::function render;
		} hooks;

		std::filesystem::path path = "";
		bool inited = false;
		bool firstFrame = true;
		core::Entity entity;
	};
}// namespace fif::lua_scripting
