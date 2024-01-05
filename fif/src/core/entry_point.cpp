#include "fif/core/entry_point.hpp"

using namespace fif::core;

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
	std::unique_ptr<Application> app(get_application());
	app->start();
}
