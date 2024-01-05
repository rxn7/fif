#pragma once

namespace fif::core {
	typedef std::function<void(const ApplicationStatus &status, entt::registry &registry)> RenderSystem;
	typedef std::function<void(const ApplicationStatus &status, entt::registry &registry, const f32 dt)> UpdateSystem;
	typedef std::function<void(const ApplicationStatus &status, entt::registry &registry, Event &event)> EventSystem;
}// namespace fif::core
