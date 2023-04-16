#pragma once

#include "fif/gfx/camera.h"
#include "fif/gfx/shader.h"
#include "fif/gfx/vertex.h"
#include "fif/gfx/vertexBuffer.h"

#include <cinttypes>
#include <vector>

namespace fif::gfx {
	class Batch {
	public:
		Batch(std::uint32_t verticesPerInstance, std::uint32_t elementsPerInstance, std::uint32_t size);
		void render();

		inline std::uint32_t getVertexCount() const {
			return m_Vertices.size();
		}
		inline std::uint32_t getElementCount() const {
			return m_Elements.size();
		}
		inline void addVertex(const Vertex &vertex) {
			m_Vertices.push_back(vertex);
		}
		inline void addElement(std::uint16_t element) {
			m_Elements.push_back(element);
		}

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<std::uint16_t> m_Elements;
		VertexBuffer m_Buffer;
	};
}