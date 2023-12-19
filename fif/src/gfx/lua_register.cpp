#include "./lua_register.hpp"
#include "fif/gfx/color.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"

namespace fif::gfx {
#ifdef FIF_LUA_SCRIPTING
	void register_lua_components() {
		lua_scripting::LuaScriptingModule *lua_module = lua_scripting::LuaScriptingModule::get_instance();

		lua_module->m_Lua.new_usertype<Color>("Color", sol::call_constructor, sol::factories([](u8 r, u8 g, u8 b, u8 a) { return Color(r, g, b, a); }), "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);

		lua_module->register_component<SpriteComponent>("SpriteComponent", "tint", &SpriteComponent::tint, "size", &SpriteComponent::size);
		lua_module->register_component<QuadComponent>("QuadComponent", "tint", &QuadComponent::tint, "size", &QuadComponent::size);
		lua_module->register_component<CircleComponent>("CircleComponent", "tint", &CircleComponent::tint, "radius", &CircleComponent::radius);
	}
#else
	void register_lua_components() {
	}
#endif
}// namespace fif::gfx
