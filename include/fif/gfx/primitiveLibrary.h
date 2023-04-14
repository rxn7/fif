#pragma once

#include "fif/gfx/shader.h"
#include "fif/gfx/primitiveType.h"
#include "fif/gfx/renderable.h"

#include <memory>

namespace fif::gfx {
	namespace PrimitiveLibrary {
		void init();
		void add(PrimitiveType type, const std::shared_ptr<Renderable> &renderable);
		std::shared_ptr<Renderable> get(PrimitiveType type);
	}
}
