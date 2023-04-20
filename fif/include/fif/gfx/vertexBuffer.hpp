#pragma once

#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex.hpp"

#include <memory>

namespace fif::gfx {
	class VertexBuffer {
	  public:
		VertexBuffer(const Vertex *vertices, std::uint32_t vertexCount,
					 const std::uint16_t *elements, std::uint32_t elementCount);
		VertexBuffer(std::uint32_t vertexCount, std::uint32_t elementCount);

		void render() const;
		void setVertices(const Vertex *vertices, std::uint32_t count);
		void setElements(const std::uint16_t *elements, std::uint32_t count);

	  private:
		static void setupVertexAttributes();

	  protected:
		std::uint32_t m_VertexCount;
		std::uint32_t m_ElementCount;
		std::uint32_t m_Vao;
		std::uint32_t m_Vbo;
		std::uint32_t m_Ebo;
	};
} // namespace fif::gfx
