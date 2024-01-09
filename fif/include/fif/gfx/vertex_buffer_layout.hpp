#pragma once

#include "glsl_data_type.hpp"

namespace fif::gfx {
	struct VertexBufferElement final {
		VertexBufferElement(GlslDataType type, const bool normalized, const bool asFloat) : type(type), normalized(normalized), asFloat(asFloat), size(get_glsl_data_type_size(type)) {}

		GlslDataType type;
		bool normalized;
		bool asFloat;

		u32 size;
		u32 offset;
	};

	class VertexBufferLayout final {
	public:
		VertexBufferLayout() {}
		VertexBufferLayout(std::initializer_list<VertexBufferElement> elements);

		u32 get_stride() const { return m_Stride; }

		const std::vector<VertexBufferElement> &get_elements() const { return m_Elements; }

		inline std::vector<VertexBufferElement>::iterator begin() { return m_Elements.begin(); }

		inline std::vector<VertexBufferElement>::iterator end() { return m_Elements.end(); }

		inline std::vector<VertexBufferElement>::const_iterator begin() const { return m_Elements.begin(); }

		inline std::vector<VertexBufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void recalculate();

	private:
		std::vector<VertexBufferElement> m_Elements;
		u32 m_Stride;
	};
}// namespace fif::gfx
