#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	VertexBufferLayout::VertexBufferLayout(std::initializer_list<VertexBufferElement> elements) : m_Elements(elements) {
		recalculate();
	}

	void VertexBufferLayout::recalculate() {
		u32 offset = 0;
		for(VertexBufferElement &el : m_Elements) {
			el.offset = offset;
			offset += el.size;
		}
		m_Stride = offset;
	}
}// namespace fif::gfx
