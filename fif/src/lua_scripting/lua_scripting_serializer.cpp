#include "./lua_scripting_serializer.hpp"
#include "fif/core/project.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"

namespace fif::lua_scripting {
	void LuaScriptingSerializer::serialize(const core::Entity &entity, YAML::Emitter &emitter) {
		serialize_component<LuaScriptComponent>(entity, emitter, [&emitter](LuaScriptComponent &luaScriptComponent) { emitter << YAML::Key << "Path" << YAML::Value << luaScriptComponent.path; });
	}

	void LuaScriptingSerializer::deserialize(core::Entity &entity, const YAML::Node &entityNode) {
		try_get_component_node<LuaScriptComponent>(entityNode, [&entity](const YAML::Node &luaScriptComponentNode) {
			const std::string path = luaScriptComponentNode["Path"].as<std::string>();
			LuaScriptingModule::get_instance()->attach_script(entity, path);
		});
	}
}// namespace fif::lua_scripting
