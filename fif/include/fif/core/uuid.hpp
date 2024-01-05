#pragma once

namespace fif::core {
	class UUID final {
	public:
		UUID();
		UUID(const u64 value);

		inline operator u64() const {
			return m_Value;
		}

		inline u64 get_value() const {
			return m_Value;
		}

	private:
		u64 m_Value;
	};
}// namespace fif::core
