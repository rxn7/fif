#include "fif/gfx/batch.hpp"
#include "fif/core/profiler.hpp"

namespace fif::gfx {
	Batch::Batch(u32 verticesPerInstance, u32 elementsPerInstance, u32 size)
		: m_Buffer(verticesPerInstance * size, elementsPerInstance * size), m_Vertices(verticesPerInstance * size),
		  m_Elements(elementsPerInstance * size) {}

	void Batch::render() {
		FIF_PROFILE_FUNC();

		if (m_Vertices.empty() || m_Elements.empty()) {
			return;
		}

		m_Buffer.setVertices(m_Vertices.data(), m_Vertices.size());
		m_Buffer.setElements(m_Elements.data(), m_Elements.size());

		m_Buffer.render();

		m_Vertices.clear();
		m_Elements.clear();
	}
} // namespace fif::gfx
