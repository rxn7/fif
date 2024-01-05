#pragma once

#include <fif/gfx/gfx_module.hpp>
#include <fif/imgui/imgui_module.hpp>
#include <fif/input/input_module.hpp>
#include <fif/lua_scripting/lua_scripting_module.hpp>
#include <fif/native_scripting/native_scripting_module.hpp>

using namespace fif;
using namespace fif::core;
using namespace fif::gfx;
using namespace fif::input;
using namespace fif::imgui;
using namespace fif::lua_scripting;
using namespace fif::native_scripting;

namespace fifed {
	class Fifed : public Application {
	public:
		Fifed();
		void setup_modules() override;
	};
}// namespace fifed
