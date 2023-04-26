#pragma once

#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex.hpp"

#include <memory>

namespace fif::gfx {
	class VertexBuffer {
	public:
		VertexBuffer(const Vertex *vertices, u32 vertexCount, const u16 *elements, u32 elementCount);
		VertexBuffer(u32 vertexCount, u32 elementCount);

		void render() const;
		void set_vertices(const Vertex *vertices, u32 count);
		void set_elements(const u16 *elements, u32 count);

	private:
		static void setup_vertex_attributes();

	protected:
		u32 m_VertexCount;
		u32 m_ElementCount;
		u32 m_Vao;
		u32 m_Vbo;
		u32 m_Ebo;
	};
}// namespace fif::gfx