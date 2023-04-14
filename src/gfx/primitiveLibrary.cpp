#include "fif/gfx/primitiveLibrary.h"

#include "fif/gfx/primitiveType.h"
#include "fif/core/assertion.h"
#include "fif/gfx/shaderLibrary.h"
#include "primitives/quad.h"

#include <memory>

namespace fif::gfx {
	static std::unordered_map<PrimitiveType, std::shared_ptr<Renderable>> s_Primitives;

	void PrimitiveLibrary::init() {
		add(PrimitiveType::QUAD, std::make_shared<Renderable>(priv::primitives::Quad::VERTICES, priv::primitives::Quad::ELEMENTS, ShaderLibrary::get("quad")));
		add(PrimitiveType::CIRCLE, std::make_shared<Renderable>(priv::primitives::Quad::VERTICES, priv::primitives::Quad::ELEMENTS, ShaderLibrary::get("circle")));
	}

	std::shared_ptr<Renderable> PrimitiveLibrary::get(PrimitiveType type) {
		auto it = s_Primitives.find(type);
		FIF_ASSERT(it != s_Primitives.end(), "Primitive with type '" << type << "' doesn't exists");
		return it->second;
	}

	void PrimitiveLibrary::add(PrimitiveType type, const std::shared_ptr<Renderable> &primitive) {
		FIF_ASSERT(primitive.get() != nullptr, "The primitive cannot be nullptr");
		s_Primitives.insert({type, primitive});
	}
}
