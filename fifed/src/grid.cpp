#include "grid.hpp"

#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex.hpp"
#include "fif/gfx/vertex_buffer.hpp"
#include "shaders/grid.hpp"

#include <array>

constexpr std::array<fif::gfx::Vertex, 4> VERTICES = {
	fif::gfx::Vertex{
		.position = {-1.0f, -1.0f},
		.uv = {-1.0f, -1.0f},
	},
	fif::gfx::Vertex{
		.position = {-1.0f, 1.0f},
		.uv = {-1.0f, 1.0f},
	},
	fif::gfx::Vertex{
		.position = {1.0f, 1.0f},
		.uv = {1.0f, 1.0f},
	},
	fif::gfx::Vertex{
		.position = {1.0f, -1.0f},
		.uv = {1.0f, -1.0f},
	},
};

constexpr std::array<u16, 6> ELEMENTS = {0u, 1u, 2u, 2u, 3u, 0u};

std::unique_ptr<fif::gfx::VertexBuffer> sp_VertexBuffer = nullptr;
std::unique_ptr<fif::gfx::Shader> sp_Shader = nullptr;

void Grid::init() {
	sp_Shader = std::make_unique<fif::gfx::Shader>(shaders::Grid::VERTEX, shaders::Grid::FRAGMENT);
	sp_VertexBuffer = std::make_unique<fif::gfx::VertexBuffer>(VERTICES.data(), VERTICES.size(), ELEMENTS.data(), ELEMENTS.size());
}

void Grid::render() {
	if(!enabled)
		return;

	const fif::gfx::OrthoCamera &cam = fif::gfx::Renderer2D::get_camera();

	sp_Shader->bind();
	sp_Shader->set_uniform("u_CameraSize", cam.get_size());
	sp_Shader->set_uniform("u_CameraPosition", cam.m_Position);
	sp_Shader->set_uniform("u_LineColor", lineColor);
	sp_Shader->set_uniform("u_LineThickness", lineThickness);
	sp_Shader->set_uniform("u_CellSize", cellSize);

	sp_VertexBuffer->render();
	sp_Shader->unbind();
}
