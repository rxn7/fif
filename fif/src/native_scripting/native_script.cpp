#include "fif/native_scripting/native_script.hpp"

namespace fif::native_scripting {
	NativeScript::NativeScript(core::Scene *scene, core::EntityID id) : m_Entity(scene, id) {}
}// namespace fif::native_scripting
