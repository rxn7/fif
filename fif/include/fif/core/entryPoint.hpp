#pragma once

#include "fif/core/application.hpp"

extern fif::core::Application *getApplication();

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
	fif::core::Application *app = getApplication();
	app->start();
	delete app;
}
