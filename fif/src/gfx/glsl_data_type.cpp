#include "fif/gfx/glsl_data_type.hpp"

namespace fif::gfx {
	u32 get_glsl_data_type_size(GlslDataType type) {
		switch(type) {
		case GlslDataType::Float:
			return 4;

		case GlslDataType::Float2:
			return 4 * 2;

		case GlslDataType::Float3:
			return 4 * 3;

		case GlslDataType::Float4:
			return 4 * 4;

		case GlslDataType::Mat3:
			return 4 * 3 * 3;

		case GlslDataType::Mat4:
			return 4 * 4 * 4;

		case GlslDataType::Int:
			return 4;

		case GlslDataType::Int2:
			return 4 * 2;

		case GlslDataType::Int3:
			return 4 * 3;

		case GlslDataType::Int4:
			return 4 * 4;

		case GlslDataType::UByte:
			return 1;

		case GlslDataType::UByte2:
			return 2;

		case GlslDataType::UByte3:
			return 3;

		case GlslDataType::UByte4:
			return 4;

		case GlslDataType::Bool:
			return 1;
		}

		FIF_LOG_ERROR("Unknown GlslDataType: " << (int)type);

		return 0;
	}

	u32 get_glsl_data_type_component_count(GlslDataType type) {
		switch(type) {
		case GlslDataType::Float:
			return 1;

		case GlslDataType::Float2:
			return 2;

		case GlslDataType::Float3:
			return 3;

		case GlslDataType::Float4:
			return 4;

		case GlslDataType::Mat3:
			return 3;

		case GlslDataType::Mat4:
			return 4;

		case GlslDataType::Int:
			return 1;

		case GlslDataType::Int2:
			return 2;

		case GlslDataType::Int3:
			return 3;

		case GlslDataType::Int4:
			return 4;

		case GlslDataType::UByte:
			return 1;

		case GlslDataType::UByte2:
			return 2;

		case GlslDataType::UByte3:
			return 3;

		case GlslDataType::UByte4:
			return 4;

		case GlslDataType::Bool:
			return 1;
		}

		FIF_LOG_ERROR("Unknown GlslDataType: " << (int)type);

		return 0;
	}
}// namespace fif::gfx
