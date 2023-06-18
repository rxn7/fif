#pragma once

#include "ecs/entity.hpp"
#include "sol/sol.hpp"

namespace fif::lua_scripting {
	struct LuaScriptComponent final {
		LuaScriptComponent(const core::Entity &ent) : entity(ent) {}

		sol::table self;
		struct {
			sol::function update;
			sol::function render;
		} hooks;

		std::filesystem::path filepath;
		bool inited = false;
		core::Entity entity;
	};
}// namespace fif::lua_scripting
