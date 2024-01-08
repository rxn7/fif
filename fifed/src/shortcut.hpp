#pragma once

namespace fifed {
	class Shortcut final {
	public:
		Shortcut(i32 key, i32 modifierBits, const std::string_view &description, const std::function<void()> &callback);

		inline void callback() const { m_Callback(); }
		inline i32 get_key() const { return m_Key; }
		inline i32 get_modifier_bits() const { return m_ModifierBits; }
		const std::string &get_description() const { return m_Description; }

	private:
		i32 m_Key;
		i32 m_ModifierBits;
		std::string m_Description;
		const std::function<void()> m_Callback;
	};
}// namespace fifed
