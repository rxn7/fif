#pragma once

namespace fif::native_scripting {
	class NativeScript {
	public:
		virtual void on_create() {}
		virtual void on_update([[maybe_unused]] f32 dt) {}
		virtual void on_render() {}

		template<typename T> bool &has_component() { mp_Scene->has_component<T>(m_EntityID); }
		template<typename T> T &get_component() { mp_Scene->get_component<T>(m_EntityID); }
		template<typename T, typename... Args> T &add_component(Args &&...args) { mp_Scene->add_component<T>(m_EntityID, std::forward(args)...); }

	protected:
		core::Scene *mp_Scene;
		core::EntityID m_EntityID;

		friend class NativeScriptingModule;
	};
}// namespace fif::native_scripting
