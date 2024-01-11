#pragma once

#include "fif/core/system.hpp"
#include "fif/native_scripting/components/native_script_component.hpp"

namespace fif::native_scripting {
	class NativeScriptingModule final : public fif::core::Module {
	public:
		FIF_MODULE(NativeScriptingModule)

		NativeScriptingModule();

		template<typename T> T &attach_script(core::EntityID ent, core::Scene &scene) {
			static_assert(std::is_base_of<NativeScript, T>().value);

			NativeScriptComponent &nativeScriptComponent = scene.add_component<NativeScriptComponent>(ent);
			nativeScriptComponent.p_script = std::make_unique<T>(scene, ent);
			nativeScriptComponent.scriptName = core::System::get_type_name<T>();
			nativeScriptComponent.p_script->on_create();

			return *static_cast<T *>(nativeScriptComponent.p_script.get());
		}
	};
}// namespace fif::native_scripting
