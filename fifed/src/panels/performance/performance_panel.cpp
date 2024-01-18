#include "performance_panel.hpp"

#include <fif/gfx/gfx_module.hpp>
#include <fif/gfx/renderer2d.hpp>

namespace fifed {
	PerformancePanel::PerformancePanel(Editor &editor) : EditorPanel(editor) {}

	void PerformancePanel::on_render() {
		const PerformanceStats &stats = Application::get_instance().get_performance_stats();
		ImGui::Text("FPS: %u", stats.fps);
		ImGui::Text("Frame time: %0.2fms", stats.frameTimeMs);
		ImGui::Text("CPU time: %0.2fms (%.0f%%)", stats.cpuTimeMs, stats.cpuTimeMs / stats.frameTimeMs * 100);
		ImGui::Text("GPU time: %0.2fms (%.0f%%)", stats.gpuTimeMs, stats.gpuTimeMs / stats.frameTimeMs * 100);

		if(ImGui::TreeNode("Renderer2D")) {
			const Renderer2DStats &rendererStats = GfxModule::get_instance().get_renderer2D().get_stats();

			if(ImGui::TreeNode("Details")) {
				ImGui::Text("Render Commands: %u", rendererStats.renderCommands);
				ImGui::Text("Draw Calls: %u", rendererStats.drawCallCount);
				ImGui::Text("Batches flushed: %u", rendererStats.batchesFlushed);
				ImGui::Text("Vertices: %u", rendererStats.vertexCount);
				ImGui::Text("Elements: %u", rendererStats.elementCount);
				ImGui::TreePop();
			}

			ImGui::Separator();

			if(ImGui::TreeNode("Objects rendered")) {
				ImGui::Text("Sprites: %u", rendererStats.spriteCount);
				ImGui::Text("Rotated sprites: %u", rendererStats.rotatedSpriteCount);
				ImGui::Text("Circles: %u", rendererStats.circleCount);
				ImGui::Text("Quads: %u", rendererStats.quadCount);
				ImGui::Text("Rotated quads: %u", rendererStats.rotatedQuadCount);
				ImGui::Text("Glyphs: %u", rendererStats.glyphCount);
				ImGui::Text("Rotated glyphs: %u", rendererStats.rotatedGlyphCount);
				ImGui::Text("Textures: %u", rendererStats.textures);
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if(ImGui::TreeNode("Lua")) {
			ImGui::Text("Memory used: %g kb", static_cast<f32>(LuaScriptingModule::get_instance().get_lua_memory_usage()) * 0.001f);
			ImGui::TreePop();
		}
	}
}// namespace fifed
