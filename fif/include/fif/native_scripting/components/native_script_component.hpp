#pragma once

#include "fif/native_scripting/native_script.hpp"

namespace fif::native_scripting {
	struct NativeScriptComponent final {
		std::string scriptName;
		std::unique_ptr<NativeScript> p_script;
	};
}// namespace fif::native_scripting
