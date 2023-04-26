#include "grid_renderer_component.hpp"
#include "../shaders/grid.hpp"

#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/profiler.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "gfx_module.hpp"

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

std::unique_ptr<fif::gfx::Shader> GridRendererComponent::sp_Shader = nullptr;

GridRendererComponent::GridRendererComponent() : m_VertexBuffer(VERTICES.data(), VERTICES.size(), ELEMENTS.data(), ELEMENTS.size()) {
	if(!sp_Shader)
		sp_Shader = std::make_unique<fif::gfx::Shader>(shaders::Grid::VERTEX, shaders::Grid::FRAGMENT);
}

void GridRendererComponent::on_render() {
	FIF_PROFILE_FUNC();

	if(!m_Enabled)
		return;

	const fif::gfx::OrthoCamera &cam = fif::gfx::Renderer2D::get_camera();

	sp_Shader->bind();
	sp_Shader->set_uniform("u_CameraSize", cam.get_size());
	sp_Shader->set_uniform("u_CameraPosition", cam.m_Position);
	sp_Shader->set_uniform("u_LineColor", m_LineColor);
	sp_Shader->set_uniform("u_LineThickness", m_LineThickness);
	sp_Shader->set_uniform("u_CellSize", m_CellSize);

	m_VertexBuffer.render();
	sp_Shader->unbind();
}
