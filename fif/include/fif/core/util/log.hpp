#pragma once

#define FIF_ANSI_RESET "\033[m"
#define FIF_ANSI_RED "\033[0;31m"
#define FIF_ANSI_YELLOW "\033[0;33m"
#define FIF_ANSI_BLUE "\033[0;34m"
#define FIF_ANSI_PURPLE "\033[0;35m"
#define FIF_ANSI_CYAN "\033[0;36m"

#define FIF_LOG(msg) std::cout << FIF_ANSI_BLUE << "[LOG] " << msg << FIF_ANSI_RESET << "\n"
#define FIF_LOG_ERROR(msg) std::cout << FIF_ANSI_RED << "[ERROR] " << msg << FIF_ANSI_RESET << "\n"
#define FIF_LOG_WARNING(msg) std::cout << FIF_ANSI_YELLOW << "[WARNING] " << msg << FIF_ANSI_RESET << "\n"
