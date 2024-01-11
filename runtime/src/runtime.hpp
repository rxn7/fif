using namespace fif;
using namespace fif::core;
using namespace fif::gfx;
using namespace fif::input;
using namespace fif::lua_scripting;

namespace fif_runtime {
	class Runtime : public Application {
	public:
		Runtime();
		void setup_modules() override;
	};
}// namespace fif_runtime
