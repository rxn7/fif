#pragma once

#include <string>

namespace fif::gfx {
	class Shader {
	public:
		Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
		~Shader();

		void bind() const;
		static void unbind();

	private:
		static std::uint32_t compile(int type, const char *src);

	private:
		std::uint32_t m_ID;
	};
}
