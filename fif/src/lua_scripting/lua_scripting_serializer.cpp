#include "./lua_scripting_serializer.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"

namespace fif::lua_scripting {
	void LuaScriptingSerializer::serialize(const core::Entity &entity, YAML::Emitter &emitter) {
		serialize_component<LuaScriptComponent>(entity, emitter, [&emitter](LuaScriptComponent &luaScriptComponent) { emitter << YAML::Key << "Path" << YAML::Value << luaScriptComponent.filepath; });
	}

	void LuaScriptingSerializer::deserialize(core::Entity &entity, const YAML::Node &entityNode) {
		try_get_component_node<LuaScriptComponent>(
			entityNode, [&entity](const YAML::Node &luaScriptComponentNode) { LuaScriptingModule::get_instance()->attach_script(entity, luaScriptComponentNode["Path"].as<std::string>()); });
	}
}// namespace fif::lua_scripting
