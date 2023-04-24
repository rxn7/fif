#pragma once

#include "fif/core/event/event.hpp"

#include <string_view>

namespace fif::core {
	class Entity;

	class Component {
		friend Entity;

	public:
		virtual ~Component() {}

		virtual constexpr const char *get_name() const = 0;
		virtual void on_create() {}
		virtual void on_update() {}
		virtual void on_render() {}
		virtual void on_event([[maybe_unused]] Event &event) {}

	protected:
		Entity *mp_Entity;
	};
}// namespace fif::core

#define COMPONENT_NAME(x)                                                                                                                            \
	constexpr const char *get_name() const override { return #x; }
