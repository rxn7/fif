#pragma once

#include <vector>
#include "fif/gfx/vertex.h"

namespace fif::gfx::priv::primitives {
	namespace Quad {
		const fif::gfx::Vertex VERTICES[4] = {
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

		const std::uint16_t ELEMENTS[6] = { 0,1,2, 2,3,0 };
	}
}
