#pragma once

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
