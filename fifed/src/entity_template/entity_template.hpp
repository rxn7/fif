#pragma once

namespace fifed {
	class EntityTemplate {
	public:
		EntityID create(Scene &scene);
		virtual ~EntityTemplate() = default;
		virtual std::string_view get_name() const = 0;

	protected:
		virtual void create_components(EntityID ent, Scene &scene) = 0;
	};
}// namespace fifed

#define ENTITY_TEMPLATE_NAME(name)                                                                                                                   \
	std::string_view get_name() const override { return name; }
