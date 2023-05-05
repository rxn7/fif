#pragma once

#include "glsl_data_type.hpp"
#include <initializer_list>

namespace fif::gfx {
	struct VertexBufferElement {
		VertexBufferElement(GlslDataType type, std::string name, bool normalized) :
			type(type), name(name), normalized(normalized), size(get_glsl_data_type_size(type)) {}

		GlslDataType type;
		std::string name;
		bool normalized;

		u32 size;
		u32 offset;
	};

	class VertexBufferLayout {
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
