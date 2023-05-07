#pragma once

namespace fif::gfx {
	enum class GlslDataType {
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		UInt,
		UInt2,
		UInt3,
		UInt4,
		UByte,
		UByte2,
		UByte3,
		UByte4,
		Bool
	};

	u32 get_glsl_data_type_size(GlslDataType type);
	u32 get_glsl_data_type_component_count(GlslDataType type);
	GLenum glsl_data_type_to_opengl_enum(GlslDataType type);
};// namespace fif::gfx
