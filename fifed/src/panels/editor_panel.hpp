#pragma once

#include "../common.hpp"
#include "event/event.hpp"
#include "imgui.h"

class EditorPanel {
public:
	virtual const char *get_title() const = 0;
	virtual void on_update([[maybe_unused]] float dt) {}
	virtual void on_event([[maybe_unused]] Event &event) {}
	void render();

private:
	virtual void on_render() {}
};
