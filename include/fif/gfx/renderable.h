#pragma once

#include "fif/gfx/shader.h"
#include "fif/gfx/vertex.h"

#include <vector>
#include <memory>

namespace fif::gfx {
	class Renderable {
	public:
		Renderable(const std::vector<Vertex> &vertices, const std::vector<std::uint16_t> &elements, const std::shared_ptr<Shader> &shader);
		void render() const;

	protected:
		std::shared_ptr<Shader> mp_Shader;
		glm::u8vec4 m_Color;
		std::uint16_t m_ElementCount;
		std::uint32_t m_Vao;
		std::uint32_t m_Vbo;
		std::uint32_t m_Ebo;
	};
}
