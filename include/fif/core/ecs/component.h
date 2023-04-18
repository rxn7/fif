#pragma once

#include "fif/core/event/event.h"

#include <string_view>

namespace fif::core {
	class Entity;

	class Component {
	friend Entity;
	public:
		virtual ~Component() { }

		virtual constexpr const char *getName() const = 0;
		virtual void onCreate() { }
		virtual void onUpdate([[maybe_unused]] float dt) { }
		virtual void onRender() { }
		virtual void onEvent([[maybe_unused]] Event &event) { }

	protected:
		Entity *mp_Entity;
	};
}

#define COMPONENT_NAME(x) constexpr const char *getName() const override { return #x; }
