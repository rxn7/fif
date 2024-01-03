#include "performance_panel.hpp"

#include "fif/gfx/renderer2d.hpp"
#include "gfx_module.hpp"

namespace fifed {
	PerformancePanel::PerformancePanel(Editor &editor) : EditorPanel(editor) {
	}

	void PerformancePanel::on_render() {
		const PerformanceStats &stats = Application::get_instance()->get_performance_stats();
		ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
		ImGui::Text("FPS: %u", stats.fps);

		if(ImGui::TreeNode("Renderer2D")) {
			const Renderer2DStats &rendererStats = GfxModule::get_instance()->get_renderer2D().get_stats();

			ImGui::SeparatorText("Details");
			ImGui::Text("Draw Calls: %u", rendererStats.drawCallCount);
			ImGui::Text("Batches flushed: %u", rendererStats.batchesFlushed);
			ImGui::Text("Vertices: %u", rendererStats.vertexCount);
			ImGui::Text("Elements: %u", rendererStats.elementCount);

			ImGui::SeparatorText("Objects rendered");
			ImGui::Text("Sprites: %u", rendererStats.spriteCount);
			ImGui::Text("Rotated sprites: %u", rendererStats.rotatedSpriteCount);
			ImGui::Text("Circles: %u", rendererStats.circleCount);
			ImGui::Text("Quads: %u", rendererStats.quadCount);
			ImGui::Text("Rotated quads: %u", rendererStats.rotatedQuadCount);
			ImGui::Text("Glyphs: %u", rendererStats.glyphCount);
			ImGui::Text("Textures: %u", rendererStats.textures);

			ImGui::TreePop();
		}

		if(ImGui::TreeNode("Lua")) {
			ImGui::Text("Memory used: %g kb", static_cast<float>(LuaScriptingModule::get_instance()->get_lua_memory_usage()) * 0.001f);
			ImGui::TreePop();
		}
	}
}// namespace fifed
