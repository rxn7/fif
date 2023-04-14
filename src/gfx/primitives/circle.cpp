#include "fif/gfx/primitives/circle.h"
#include "fif/gfx/shaderLibrary.h"
#include <memory>

namespace fif::gfx::primitives {
	const std::vector<fif::gfx::Vertex> VERTICES = { 
		{
			.position = {-0.5f, -0.5f, 0.0f},
			.uv = {0.0f, 0.0f},
		},
		{
			.position = { 0.5f, -0.5f, 0.0f},
			.uv = {1.0f, 0.0f},
		},
		{
			.position = { 0.5f,  0.5f, 0.0f},
			.uv = {1.0f, 1.0f},
		},
		{
			.position = {-0.5f,  0.5f, 0.0f},
			.uv = {0.0f, 1.0f},
		} 
	};

	const std::vector<std::uint16_t> ELEMENTS = {
		0,1,2,
		2,3,0
	};

	CirclePrimitive::CirclePrimitive() : Renderable(VERTICES, ELEMENTS, ShaderLibrary::get("circle")) { }
}
