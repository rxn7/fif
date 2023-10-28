#pragma once

namespace fif::native_scripting {
	class NativeScript {
	public:
		NativeScript(core::Scene &scene, core::EntityID id);

		virtual void on_create() {}
		virtual void on_update([[maybe_unused]] f32 dt) {}
		virtual void on_render() {}

	protected:
		core::Entity m_Entity;
		friend class NativeScriptingModule;
	};
}// namespace fif::native_scripting
