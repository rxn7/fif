#include "fif/gfx/batch.hpp"

namespace fif::gfx {
	Batch::Batch(u32 verticesPerInstance, u32 elementsPerInstance, u32 size)
		: m_Buffer(verticesPerInstance * size, elementsPerInstance * size), m_Vertices(verticesPerInstance * size), m_Elements(elementsPerInstance * size) {}

	void Batch::render() {
		if(m_Vertices.empty() || m_Elements.empty())
			return;

		m_Buffer.set_vertices(m_Vertices.data(), m_Vertices.size());
		m_Buffer.set_elements(m_Elements.data(), m_Elements.size());

		m_Buffer.render();

		m_Vertices.clear();
		m_Elements.clear();
	}
}// namespace fif::gfx
