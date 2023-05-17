#pragma once

#include "fif/gfx/shader.hpp"
#include "vertex_buffer_layout.hpp"

namespace fif::gfx {
	class VertexBuffer {
	public:
		VertexBuffer(const void *vertices, u32 vertexCount, const u16 *elements, u32 elementCount, u32 m_VertexSize, const VertexBufferLayout &layout);
		VertexBuffer(u32 vertexCount, u32 elementCount, u32 m_VertexSize, const VertexBufferLayout &layout);

		void render() const;
		void set_vertices_and_elements(const void *vertices, u32 vertexCount, const u16 *elements, u32 elementCount);
		void set_layout(const VertexBufferLayout &layout);
		const VertexBufferLayout &get_layout() const { return m_Layout; }

	private:
		void setup_vertex_attributes();

	protected:
		const VertexBufferLayout &m_Layout;
		u32 m_VertexCount;
		u32 m_ElementCount;
		u32 m_VertexSize;
		u32 m_Vao;
		u32 m_Vbo;
		u32 m_Ebo;
	};
}// namespace fif::gfx
