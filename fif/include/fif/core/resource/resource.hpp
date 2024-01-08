#pragma once

#include <filesystem>

namespace fif::core {

// NOTE: Resource type is stored as std::string_view instead of enum because other modules may declare their own resources.
#define FIF_RESOURCE(T)                                                                                                                                                                                \
	static inline constexpr std::string get_type_static() { return (T); }                                                                                                                              \
                                                                                                                                                                                                       \
	inline constexpr std::string get_type() const override { return get_type_static(); }

	class Resource {
	public:
		std::filesystem::path get_path_relative() const;

		inline std::filesystem::path get_path() const { return m_Path; }

		inline UUID get_uuid() const { return m_UUID; }

		virtual inline constexpr std::string get_type() const = 0;

	protected:
		Resource() {}
		Resource(const std::filesystem::path &path);

	protected:
		bool m_IsEditorResource = false;

		UUID m_UUID;
		std::filesystem::path m_Path;

		friend class ResourceManager;
	};
}// namespace fif::core
