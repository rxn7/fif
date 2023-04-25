#pragma once

#include "fif/core/ecs/component.hpp"
#include "fif/core/event/event.hpp"
#include "fif/gfx/color.hpp"
#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex_buffer.hpp"

#include "glm/vec2.hpp"

class GridRendererComponent final : public fif::core::Component {
public:
	GridRendererComponent();
	COMPONENT_NAME(GridRendererComponent);
	void on_render() override;

public:
	bool m_Enabled = true;
	fif::gfx::Color m_LineColor = fif::gfx::Color(20, 20, 20, 255);
	f32 m_LineThickness = 0.7f;
	f32 m_CellSize = 10.0f;

private:
	fif::gfx::VertexBuffer m_VertexBuffer;
	static std::unique_ptr<fif::gfx::Shader> sp_Shader;
};
