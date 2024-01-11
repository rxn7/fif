#include "./lua_register.hpp"

#ifdef FIF_LUA_SCRIPTING
#include "fif/gfx/color.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/label_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/gfx/text/text_align.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"
#endif

namespace fif::gfx {
#ifdef FIF_LUA_SCRIPTING
	void register_lua_types() {
		lua_scripting::LuaScriptingModule &luaModule = lua_scripting::LuaScriptingModule::get_instance();

		// clang-format off
		luaModule.m_Lua.new_usertype<Color>("Color",
			sol::call_constructor, sol::factories([](u8 r, u8 g, u8 b, u8 a) { return Color(r, g, b, a); }),
			"r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a,
			"random", &Colors::get_random,
			sol::meta_function::to_string, [](const Color &color) { 
				std::ostringstream ss;
				ss << "(" << std::to_string(static_cast<i32>(color.r)) << ", " << std::to_string(static_cast<i32>(color.g)) << ", " << std::to_string(static_cast<i32>(color.b)) << ", " << std::to_string(static_cast<i32>(color.a)) << ")";
				return ss.str();
			}
		);
		// clang-format on

		luaModule.m_Lua.new_enum("HorizontalTextAlign", "Left", HorizontalTextAlign::Left, "Center", HorizontalTextAlign::Center, "Right", HorizontalTextAlign::Right);
		luaModule.m_Lua.new_enum("VerticalTextAlign", "Top", VerticalTextAlign::Top, "Center", VerticalTextAlign::Center, "Bottom", VerticalTextAlign::Bottom);

		luaModule.register_component<SpriteComponent>("tint", &SpriteComponent::tint, "size", &SpriteComponent::size, "texture", &SpriteComponent::p_texture);
		luaModule.register_component<QuadComponent>("tint", &QuadComponent::tint, "size", &QuadComponent::size);
		luaModule.register_component<CircleComponent>("tint", &CircleComponent::tint, "radius", &CircleComponent::radius);
		luaModule.register_component<LabelComponent>("text", &LabelComponent::text, "size", &LabelComponent::fontSize, "color", &LabelComponent::color, "horizontalAlign", &LabelComponent::horizontalAlign, "verticalAlign", &LabelComponent::verticalAlign);
	}
#else
	void register_lua_types() {}
#endif
}// namespace fif::gfx
