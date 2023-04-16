#include "fif/gfx/batch.h"
#include "fif/core/profiler.h"

namespace fif::gfx {
	Batch::Batch(std::uint32_t verticesPerInstance, std::uint32_t elementsPerInstance, std::uint32_t size) : m_Buffer(verticesPerInstance * size, elementsPerInstance * size), m_Vertices(verticesPerInstance * size), m_Elements(elementsPerInstance * size) {
	}

	void Batch::render() {
		FIF_PROFILE_FUNC();
		m_Buffer.setVertices(m_Vertices.data(), m_Vertices.size());
		m_Buffer.setElements(m_Elements.data(), m_Elements.size());

		m_Buffer.render();

		m_Vertices.clear();
		m_Elements.clear();
	}
}
