#include "grid.hpp"
#include "common.hpp"
#include "shaders/grid.hpp"

#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex.hpp"
#include "fif/gfx/vertex_buffer.hpp"

namespace fifed {
	constexpr std::array<Vertex, 4> VERTICES = {
		Vertex{
			.position = {-1.0f, -1.0f},
			.uv = {-1.0f, -1.0f},
		},
		Vertex{
			.position = {-1.0f, 1.0f},
			.uv = {-1.0f, 1.0f},
		},
		Vertex{
			.position = {1.0f, 1.0f},
			.uv = {1.0f, 1.0f},
		},
		Vertex{
			.position = {1.0f, -1.0f},
			.uv = {1.0f, -1.0f},
		},
	};

	constexpr std::array<u16, 6> ELEMENTS = {0u, 1u, 2u, 2u, 3u, 0u};

	std::unique_ptr<VertexBuffer> sp_VertexBuffer = nullptr;
	std::unique_ptr<Shader> sp_Shader = nullptr;

	void Grid::init() {
		sp_Shader = std::make_unique<Shader>(shaders::Grid::VERTEX, shaders::Grid::FRAGMENT);
		sp_VertexBuffer = std::make_unique<VertexBuffer>(VERTICES.data(), VERTICES.size(), ELEMENTS.data(), ELEMENTS.size());
	}

	void Grid::render() {
		if(!enabled)
			return;

		const OrthoCamera &cam = Renderer2D::get_camera();

		sp_Shader->bind();
		sp_Shader->set_uniform("u_CameraSize", cam.get_size());
		sp_Shader->set_uniform("u_CameraPosition", cam.m_Position);
		sp_Shader->set_uniform("u_LineColor", lineColor);
		sp_Shader->set_uniform("u_LineThickness", lineThickness);
		sp_Shader->set_uniform("u_CellSize", cellSize);

		sp_VertexBuffer->render();
		sp_Shader->unbind();
	}
}// namespace fifed
