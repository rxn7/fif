#include "grid.hpp"
#include "color.hpp"
#include "shaders/grid.hpp"

#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex_buffer.hpp"

namespace fifed {
	constexpr std::array<Grid::GridVertex, 4> VERTICES = {
		Grid::GridVertex{
			.position = {-1.0f, -1.0f},
			.uv = {-1.0f, -1.0f},
		},
		Grid::GridVertex{
			.position = {-1.0f, 1.0f},
			.uv = {-1.0f, 1.0f},
		},
		Grid::GridVertex{
			.position = {1.0f, 1.0f},
			.uv = {1.0f, 1.0f},
		},
		Grid::GridVertex{
			.position = {1.0f, -1.0f},
			.uv = {1.0f, -1.0f},
		},
	};

	constexpr std::array<u16, 6> ELEMENTS = {0u, 1u, 2u, 2u, 3u, 0u};

	std::unique_ptr<VertexBuffer> sp_VertexBuffer = nullptr;
	std::unique_ptr<Shader> sp_Shader = nullptr;

	void Grid::init() {
		sp_Shader = std::make_unique<Shader>(shaders::Grid::VERTEX, shaders::Grid::FRAGMENT);

		sp_VertexBuffer = std::make_unique<VertexBuffer>(VERTICES.data(), VERTICES.size(), ELEMENTS.data(), ELEMENTS.size(), sizeof(Grid::GridVertex));
		sp_VertexBuffer->set_layout(gridVertexBufferLayout);
	}

	void Grid::render() {
		if(!enabled)
			return;

		const OrthoCamera &cam = GfxModule::get_instance()->get_renderer2D().get_camera();

		sp_Shader->bind();
		sp_Shader->set_uniform_vec2("u_CameraSize", cam.get_size());
		sp_Shader->set_uniform_vec2("u_CameraPosition", cam.m_Position);
		sp_Shader->set_uniform_vec3("u_LineColor", normalize_color3(lineColor));
		sp_Shader->set_uniform_vec3("u_BackgroundColor", normalize_color3(backgroundColor));
		sp_Shader->set_uniform_f32("u_LineThickness", lineThickness);
		sp_Shader->set_uniform_f32("u_MinCellSize", minCellSize);

		sp_VertexBuffer->render();
		sp_Shader->unbind();
	}
}// namespace fifed
