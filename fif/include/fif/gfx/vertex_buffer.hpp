#pragma once

#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	class VertexBuffer final {
	public:
		VertexBuffer(const void *vertices, const u32 vertexCount, const u16 *elements, const u32 elementCount, const u32 m_VertexSize, const VertexBufferLayout &layout);
		VertexBuffer(const u32 vertexCount, const u32 elementCount, const u32 m_VertexSize, const VertexBufferLayout &layout);

		void render() const;
		void set_vertices_and_elements(const void *vertices, const u32 vertexCount, const u16 *elements, const u32 elementCount);
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
