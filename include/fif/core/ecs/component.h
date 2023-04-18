#pragma once


#include "fif/core/event/event.h"
namespace fif::core {
	class Entity;

	class Component {
	friend Entity;
	public:
		virtual ~Component() { }

		virtual void onCreate() { }
		virtual void onUpdate([[maybe_unused]] float dt) { }
		virtual void onRender() { }
		virtual void onEvent([[maybe_unused]] Event &event) { }

	private:
		Entity *mp_Entity;
	};
}
