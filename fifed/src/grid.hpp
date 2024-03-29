#pragma once

#include <fif/gfx/color.hpp>
#include <fif/gfx/frame_buffer.hpp>
#include <fif/gfx/ortho_camera.hpp>
#include <fif/gfx/vertex_buffer_layout.hpp>

namespace fifed {
	struct GridVertex {
		fif::vec2 position;
		fif::vec2 uv;
	};

	class Grid final {
	public:
		Grid(const OrthoCamera &camera, const FrameBuffer &frameBuffer);
		void render();

	private:
		bool m_Enabled = true;
		Color3 m_LineColor = Color3(20, 20, 20);
		f32 m_LineThickness = 1.5f;
		f32 m_MinCellSize = 0.05f;
		VertexBuffer<GridVertex> m_VertexBuffer;
		Shader m_Shader;
		const OrthoCamera &m_Camera;
		const FrameBuffer &m_FrameBuffer;

		friend class SettingsPanel;
	};
}// namespace fifed
