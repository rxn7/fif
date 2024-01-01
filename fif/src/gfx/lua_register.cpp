#include "./lua_register.hpp"
#include "fif/gfx/color.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/label_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/gfx/text/text_align.hpp"

#ifdef FIF_LUA_SCRIPTING
#include "fif/lua_scripting/lua_scripting_module.hpp"
#endif

namespace fif::gfx {
#ifdef FIF_LUA_SCRIPTING
	void register_lua_types() {
		lua_scripting::LuaScriptingModule *luaModule = lua_scripting::LuaScriptingModule::get_instance();

		luaModule->m_Lua.new_usertype<Color>("Color", sol::call_constructor, sol::factories([](u8 r, u8 g, u8 b, u8 a) { return Color(r, g, b, a); }), "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a, "random", &Colors::get_random);

		luaModule->m_Lua.new_enum("HorizontalTextAlign", "Left", HorizontalTextAlign::Left, "Center", HorizontalTextAlign::Center, "Right", HorizontalTextAlign::Right);
		luaModule->m_Lua.new_enum("VerticalTextAlign", "Top", VerticalTextAlign::Top, "Center", VerticalTextAlign::Center, "Bottom", VerticalTextAlign::Bottom);

		luaModule->register_component<SpriteComponent>("SpriteComponent", "tint", &SpriteComponent::tint, "size", &SpriteComponent::size);
		luaModule->register_component<QuadComponent>("QuadComponent", "tint", &QuadComponent::tint, "size", &QuadComponent::size);
		luaModule->register_component<CircleComponent>("CircleComponent", "tint", &CircleComponent::tint, "radius", &CircleComponent::radius);
		luaModule->register_component<LabelComponent>("LabelComponent", "text", &LabelComponent::text, "size", &LabelComponent::fontSize, "color", &LabelComponent::color, "horizontalAlign", &LabelComponent::horizontalAlign, "verticalAlign", &LabelComponent::verticalAlign);
	}
#else
	void register_lua_components() {
	}
#endif
}// namespace fif::gfx
