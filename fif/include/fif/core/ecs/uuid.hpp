#pragma once

#include "fif/core/types.hpp"

#include <functional>

namespace fif::core {
	class UUID final {
	public:
		UUID();
		UUID(u64 uuid);

		operator u64() const { return m_UUID; }

	private:
		u64 m_UUID;
	};
}// namespace fif::core

namespace std {
	template<> struct hash<fif::core::UUID> {
		std::size_t operator()(const fif::core::UUID &uuid) const { return std::hash<u64>()(static_cast<u64>(uuid)); }
	};
}// namespace std
