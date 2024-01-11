#pragma once

#ifndef _WIN32
#include <cxxabi.h>
#endif

namespace fif::core {
	namespace System {
		void open_url(const std::string &url);
		std::string camel_case_to_snake_case(const std::string &camelCase);

		template<typename T> std::string get_type_name() {
#ifdef _WIN32
			return typeid(T).name();
#else
			char *demangledName = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, nullptr);
			std::string nameStr = demangledName;
			std::free(demangledName);

			return nameStr;
#endif
		}
	}// namespace System
}// namespace fif::core
