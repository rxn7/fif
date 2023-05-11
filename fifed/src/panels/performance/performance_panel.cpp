#include "performance_panel.hpp"

#include "fif/gfx/renderer2d.hpp"
#include "gfx_module.hpp"

namespace fifed {
	void PerformancePanel::on_render() {
		const PerformanceStats &stats = Application::get_instance()->get_performance_stats();
		ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
		ImGui::Text("FPS: %u", stats.fps);

		if(ImGui::TreeNode("Renderer2D")) {
			const Renderer2DStats &rendererStats = GfxModule::get_instance()->get_renderer2D().get_stats();
			ImGui::Text("Draw Calls: %u", rendererStats.drawCalls);
			ImGui::Text("Batches flushed: %u", rendererStats.batchesFlushed);
			ImGui::Text("Sprites: %u", rendererStats.sprites);
			ImGui::Text("Circles: %u", rendererStats.circles);
			ImGui::Text("Quads: %u", rendererStats.quads);
			ImGui::Text("Vertices: %u", rendererStats.vertices);
			ImGui::Text("Elements: %u", rendererStats.elements);
			ImGui::TreePop();
		}
	}
}// namespace fifed
