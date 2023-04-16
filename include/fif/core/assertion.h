#pragma once

#include <iostream>
#include "fif/core/log.h"

#ifndef __FUNCTION_NAME__
    #ifdef WIN32   //WINDOWS
        #define __FUNCTION_NAME__   __FUNCTION__  
    #else          //*NIX
        #define __FUNCTION_NAME__   __func__ 
    #endif
#endif

#define FIF_ASSERT(cond, msg) \
	if(!(cond)) { \
		FIF_LOG_ERROR("Assertion failed: [" << #cond << "] in file: " << __FILE__ << ", function: [" << __FUNCTION_NAME__ << "], line: " << __LINE__ << "\n" << msg << std::endl); \
		std::abort(); \
	}
