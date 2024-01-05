#include "grid.hpp"
#include "color.hpp"
#include "frame_buffer.hpp"
#include "shaders/grid.hpp"

#include <fif/gfx/renderer2d.hpp>
#include <fif/gfx/shader.hpp>
#include <fif/gfx/vertex_buffer.hpp>

namespace fifed {
	struct GridVertex {
		fif::vec2 position;
		fif::vec2 uv;
	};

	static constexpr std::array<GridVertex, 4> VERTICES = {
		GridVertex{
			.position = {-1.0f, -1.0f},
			.uv = {-1.0f, -1.0f},
		},
		GridVertex{
			.position = {-1.0f, 1.0f},
			.uv = {-1.0f, 1.0f},
		},
		GridVertex{
			.position = {1.0f, 1.0f},
			.uv = {1.0f, 1.0f},
		},
		GridVertex{
			.position = {1.0f, -1.0f},
			.uv = {1.0f, -1.0f},
		},
	};

	static constexpr std::array<u16, 6> ELEMENTS = {0u, 1u, 2u, 2u, 3u, 0u};

	static const VertexBufferLayout VERTEX_LAYOUT = {
		{GlslDataType::Float2, "a_Position", false},
		{GlslDataType::Float2, "a_UV", false},
	};

	Grid::Grid(const OrthoCamera &cam, const FrameBuffer &frameBuffer) :
		m_VertexBuffer(VERTICES.data(), VERTICES.size(), ELEMENTS.data(), ELEMENTS.size(), sizeof(GridVertex), VERTEX_LAYOUT), m_Shader(shaders::Grid::VERTEX, shaders::Grid::FRAGMENT), m_Camera(cam), m_FrameBuffer(frameBuffer) {
	}

	void Grid::render() {
		if(!m_Enabled)
			return;

		m_Shader.bind();
		m_Shader.set_uniform_vec2("u_CameraSize", m_Camera.get_size());
		m_Shader.set_uniform_vec2("u_CameraPosition", m_Camera.m_Position);
		m_Shader.set_uniform_vec3("u_LineColor", normalize_color3(m_LineColor));
		m_Shader.set_uniform_vec3("u_BackgroundColor", normalize_color3(m_FrameBuffer.m_Color));
		m_Shader.set_uniform_f32("u_LineThickness", m_LineThickness);
		m_Shader.set_uniform_f32("u_MinCellSize", m_MinCellSize);

		m_VertexBuffer.render();

		m_Shader.unbind();
	}
}// namespace fifed
