#include "shortcut.hpp"

namespace fifed {
	static const std::unordered_map<i32, const char *> MODIFIER_STR_MAP = {{GLFW_MOD_SHIFT, "Shift"}, {GLFW_MOD_CONTROL, "Ctrl"}, {GLFW_MOD_ALT, "Alt"}, {GLFW_MOD_SUPER, "Super"}, {GLFW_MOD_CAPS_LOCK, "Caps"}};

	Shortcut::Shortcut(i32 key, i32 modifierBits, const std::string_view &description, const std::function<void()> &callback) : m_Key(key), m_ModifierBits(modifierBits), m_Callback(callback) {
		std::stringstream ss;
		ss << description << ": ";

		if(modifierBits)
			for(const auto &modStr : MODIFIER_STR_MAP)
				if(modifierBits & modStr.first)
					ss << modStr.second << " + ";

		switch(key) {
		case GLFW_KEY_DELETE:
			ss << "delete";
			break;

		default:
			ss << glfwGetKeyName(key, 0);
			break;
		}

		m_Description = ss.str();
	}
}// namespace fifed
