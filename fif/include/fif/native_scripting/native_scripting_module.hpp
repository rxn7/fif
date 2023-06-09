#pragma once

#include "fif/native_scripting/components/native_script_component.hpp"

#ifndef _WIN32
#include <cxxabi.h>
#endif

namespace fif::native_scripting {
	class NativeScriptingModule : public fif::core::Module {
	public:
		FIF_MODULE(NativeScriptingModule)

		NativeScriptingModule();

		template<typename T> T &attach_script(core::EntityID ent, core::Scene &scene) {
			static_assert(std::is_base_of<NativeScript, T>().value);

			NativeScriptComponent &nativeScriptComponent = scene.add_component<NativeScriptComponent>(ent);
			nativeScriptComponent.p_script = std::make_unique<T>();
			nativeScriptComponent.p_script->m_Entity = core::Entity(&scene, ent);

#ifdef _WIN32
			nativeScriptComponent.scriptName = typeid(T).name();
#else
			char *demangledName = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, nullptr);
			nativeScriptComponent.scriptName = demangledName;
			std::free(demangledName);
#endif

			nativeScriptComponent.p_script->on_create();

			return *static_cast<T *>(nativeScriptComponent.p_script.get());
		}

	protected:
		void on_start() override;
	};
}// namespace fif::native_scripting
