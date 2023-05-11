#pragma once

namespace fif::native_scripting {
	class NativeScript {
	public:
		virtual void on_create() {}
		virtual void on_update([[maybe_unused]] f32 dt) {}
		virtual void on_render() {}

	protected:
		core::Scene *mp_Scene;
		core::EntityID m_EntityID;

		friend class NativeScriptingModule;
	};
}// namespace fif::native_scripting
