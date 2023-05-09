#pragma once

#include "fif/gfx/components/transform_component.hpp"
#include "fif/native_scripting/native_script.hpp"
#include "glm/gtc/constants.hpp"

namespace fifed {
	class SpinnerScript : public NativeScript {
	public:
		void on_update(float dt) override {
			if(!mp_Trans && !(mp_Trans = &mp_Scene->get_component<TransformComponent>(m_EntityID)))
				return;

			// Make sure it's wrapping between -360 and 360
			while(mp_Trans->angle >= glm::two_pi<f32>())
				mp_Trans->angle -= glm::two_pi<f32>();

			while(mp_Trans->angle <= -glm::two_pi<f32>())
				mp_Trans->angle += glm::two_pi<f32>();

			mp_Trans->angle -= dt;
		}

	private:
		TransformComponent *mp_Trans;
	};
}// namespace fifed
