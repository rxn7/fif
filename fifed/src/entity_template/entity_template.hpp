#pragma once

namespace fifed {
	class EntityTemplate {
	public:
		EntityID create(Scene &scene);
		virtual ~EntityTemplate() = default;
		virtual inline constexpr std::string_view get_name() const = 0;

	protected:
		virtual void create_components(Entity &ent) = 0;
	};
}// namespace fifed

#define ENTITY_TEMPLATE_NAME(name)                                                                                                                                                                     \
	inline constexpr std::string_view get_name() const override {                                                                                                                                      \
		return name;                                                                                                                                                                                   \
	}
