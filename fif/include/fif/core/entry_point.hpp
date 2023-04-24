#pragma once

#include "fif/core/application.hpp"

extern fif::core::Application *get_application();

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
	fif::core::Application *app = get_application();
	app->start();
	delete app;
}
