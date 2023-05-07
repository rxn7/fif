#include "performance_panel.hpp"
#include "../../common.hpp"

#include "fif/gfx/renderer2d.hpp"
#include "gfx_module.hpp"

namespace fifed {
	const char *PerformancePanel::get_title() const {
		return "Performance";
	}

	void PerformancePanel::on_render() {
		const PerformanceStats &stats = Application::get_instance()->get_performance_stats();
		ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
		ImGui::Text("FPS: %f", stats.fps);

		if(ImGui::TreeNode("Renderer2D")) {
			const Renderer2DStats &rendererStats = GfxModule::get_instance()->get_renderer2D().get_stats();
			ImGui::Text("Batches flushed: %i", rendererStats.batchesFlushed);
			ImGui::Text("Sprites: %i", rendererStats.sprites);
			ImGui::Text("Circles: %i", rendererStats.circles);
			ImGui::Text("Quads: %i", rendererStats.quads);
			ImGui::Text("Vertices: %i", rendererStats.vertices);
			ImGui::Text("Elements: %i", rendererStats.elements);
			ImGui::TreePop();
		}
	}
}// namespace fifed
