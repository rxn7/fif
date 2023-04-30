#include "fif/core/entry_point.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
	fif::core::Application *app = get_application();
	app->start();
	delete app;
}
