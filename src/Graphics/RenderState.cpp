#include "RenderState.h"
#include "PekanLogger.h"

#include "GLCall.h"

// Default number of samples to be used for Multisample Anti-Aliasing (MSAA)
const int DEFAULT_NUMBER_OF_SAMPLES = 8;

namespace Pekan
{
namespace Graphics
{
	bool RenderState::s_isEnabledFaceCulling = false;

	void RenderState::setBackgroundColor(float r, float g, float b, float a)
	{
		GLCall(glClearColor(r, g, b, a));
	}

	void RenderState::enableBlending()
	{
		GLCall(glEnable(GL_BLEND));
	}

	void RenderState::setBlendFunction(BlendFactor sourceFactor, BlendFactor destinationFactor)
	{
		GLCall(glBlendFunc(getBlendFactorOpenGLEnum(sourceFactor), getBlendFactorOpenGLEnum(destinationFactor)));
	}

	void RenderState::enableDepthTest()
	{
		GLCall(glEnable(GL_DEPTH_TEST));
	}

	void RenderState::enableMultisampleAntiAliasing()
	{
		GLCall(glEnable(GL_MULTISAMPLE));
	}

	void RenderState::enableFaceCulling()
	{
		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_BACK));
		GLCall(glFrontFace(GL_CCW));
		s_isEnabledFaceCulling = true;
	}

	void RenderState::disableFaceCulling()
	{
		GLCall(glDisable(GL_CULL_FACE));
		s_isEnabledFaceCulling = false;
	}

	unsigned RenderState::getShaderDataTypeOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:     return GL_FLOAT;
			case ShaderDataType::Float2:    return GL_FLOAT;
			case ShaderDataType::Float3:    return GL_FLOAT;
			case ShaderDataType::Float4:    return GL_FLOAT;
			case ShaderDataType::Mat3:      return GL_FLOAT;
			case ShaderDataType::Mat4:      return GL_FLOAT;
			case ShaderDataType::Int:       return GL_INT;
			case ShaderDataType::Int2:      return GL_INT;
			case ShaderDataType::Int3:      return GL_INT;
			case ShaderDataType::Int4:      return GL_INT;
			case ShaderDataType::Bool:      return GL_BOOL;
		}
		PK_ASSERT(false, "Unknown ShaderDataType, cannot determine OpenGL base type.", "Pekan");
		return 0;
	}

	unsigned RenderState::getBlendFactorOpenGLEnum(BlendFactor blendFactor)
	{
		switch (blendFactor)
		{
			case BlendFactor::Zero:                     return GL_ZERO;
			case BlendFactor::One:                      return GL_ONE;
			case BlendFactor::SrcColor:                 return GL_SRC_COLOR;
			case BlendFactor::OneMinusSrcColor:         return GL_ONE_MINUS_SRC_COLOR;
			case BlendFactor::DstColor:                 return GL_DST_COLOR;
			case BlendFactor::OneMinusDstColor:         return GL_ONE_MINUS_DST_COLOR;
			case BlendFactor::SrcAlpha:                 return GL_SRC_ALPHA;
			case BlendFactor::OneMinusSrcAlpha:         return GL_ONE_MINUS_SRC_ALPHA;
			case BlendFactor::DstAlpha:                 return GL_DST_ALPHA;
			case BlendFactor::OneMinusDstAlpha:         return GL_ONE_MINUS_DST_ALPHA;
			case BlendFactor::ConstantColor:            return GL_CONSTANT_COLOR;
			case BlendFactor::OneMinusConstantColor:    return GL_ONE_MINUS_CONSTANT_COLOR;
			case BlendFactor::ConstantAlpha:            return GL_CONSTANT_ALPHA;
			case BlendFactor::OneMinusConstantAlpha:    return GL_ONE_MINUS_CONSTANT_ALPHA;
		}
		PK_ASSERT(false, "Unknown BlendFactor, cannot determine OpenGL enum.", "Pekan");
		return 0;
	}

	unsigned RenderState::getBufferDataUsageOpenGLEnum(BufferDataUsage dataUsage)
	{
		switch (dataUsage)
		{
			case BufferDataUsage::StreamDraw:     return GL_STREAM_DRAW;
			case BufferDataUsage::StreamRead:     return GL_STREAM_READ;
			case BufferDataUsage::StreamCopy:     return GL_STREAM_COPY;
			case BufferDataUsage::StaticDraw:     return GL_STATIC_DRAW;
			case BufferDataUsage::StaticRead:     return GL_STATIC_READ;
			case BufferDataUsage::StaticCopy:     return GL_STATIC_COPY;
			case BufferDataUsage::DynamicDraw:    return GL_DYNAMIC_DRAW;
			case BufferDataUsage::DynamicRead:    return GL_DYNAMIC_READ;
			case BufferDataUsage::DynamicCopy:    return GL_DYNAMIC_COPY;
		}
		PK_ASSERT(false, "Unknown BufferDataUsage, cannot determine OpenGL enum.", "Pekan");
		return 0;
	}

	unsigned RenderState::getShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:     return 4;
			case ShaderDataType::Float2:    return 4 * 2;
			case ShaderDataType::Float3:    return 4 * 3;
			case ShaderDataType::Float4:    return 4 * 4;
			case ShaderDataType::Mat3:      return 4 * 3 * 3;
			case ShaderDataType::Mat4:      return 4 * 4 * 4;
			case ShaderDataType::Int:       return 4;
			case ShaderDataType::Int2:      return 4 * 2;
			case ShaderDataType::Int3:      return 4 * 3;
			case ShaderDataType::Int4:      return 4 * 4;
			case ShaderDataType::Bool:      return 1;
		}
		PK_ASSERT(false, "Unknown ShaderDataType, cannot determine its size.", "Pekan");
		return 0;
	}

	unsigned RenderState::getShaderDataTypeComponentsCount(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:     return 1;
			case ShaderDataType::Float2:    return 2;
			case ShaderDataType::Float3:    return 3;
			case ShaderDataType::Float4:    return 4;
			case ShaderDataType::Mat3:      return 3 * 3;
			case ShaderDataType::Mat4:      return 4 * 4;
			case ShaderDataType::Int:       return 1;
			case ShaderDataType::Int2:      return 2;
			case ShaderDataType::Int3:      return 3;
			case ShaderDataType::Int4:      return 4;
			case ShaderDataType::Bool:      return 1;
		}
		PK_ASSERT(false, "Unknown ShaderDataType, cannot determine its components count.", "Pekan");
		return 0;
	}

	bool RenderState::isShaderDataTypeInt(ShaderDataType type)
	{
		return
		(
			type == ShaderDataType::Int ||
			type == ShaderDataType::Int2 ||
			type == ShaderDataType::Int3 ||
			type == ShaderDataType::Int4
		);
	}

	unsigned RenderState::getTextureSlotOpenGLEnum(unsigned slot)
	{
		switch (slot)
		{
			case 0:     return GL_TEXTURE0;
			case 1:     return GL_TEXTURE1;
			case 2:     return GL_TEXTURE2;
			case 3:     return GL_TEXTURE3;
			case 4:     return GL_TEXTURE4;
			case 5:     return GL_TEXTURE5;
			case 6:     return GL_TEXTURE6;
			case 7:     return GL_TEXTURE7;
			case 8:     return GL_TEXTURE8;
			case 9:     return GL_TEXTURE9;
			case 10:    return GL_TEXTURE10;
			case 11:    return GL_TEXTURE11;
			case 12:    return GL_TEXTURE12;
			case 13:    return GL_TEXTURE13;
			case 14:    return GL_TEXTURE14;
			case 15:    return GL_TEXTURE15;
			case 16:    return GL_TEXTURE16;
			case 17:    return GL_TEXTURE17;
			case 18:    return GL_TEXTURE18;
			case 19:    return GL_TEXTURE19;
			case 20:    return GL_TEXTURE20;
			case 21:    return GL_TEXTURE21;
			case 22:    return GL_TEXTURE22;
			case 23:    return GL_TEXTURE23;
			case 24:    return GL_TEXTURE24;
			case 25:    return GL_TEXTURE25;
			case 26:    return GL_TEXTURE26;
			case 27:    return GL_TEXTURE27;
			case 28:    return GL_TEXTURE28;
			case 29:    return GL_TEXTURE29;
			case 30:    return GL_TEXTURE30;
			case 31:    return GL_TEXTURE31;
		}
		PK_ASSERT(false, "Invalid texture slot, cannot determine OpenGL enum.", "Pekan");
		return 0;
	}

	int RenderState::getMaxTextureSlots()
	{
		static int maxTextureSlots = -1;
		if (maxTextureSlots == -1)
		{
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
		}
		return maxTextureSlots;
	}

	int RenderState::getMaxTextureSize()
	{
		static int maxTextureSize = -1;
		if (maxTextureSize == -1)
		{
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		}
		return maxTextureSize;
	}

	unsigned RenderState::getTextureMinifyFunctionOpenGLEnum(TextureMinifyFunction function)
	{
		switch (function)
		{
			case TextureMinifyFunction::Nearest:                   return GL_NEAREST;
			case TextureMinifyFunction::Linear:                    return GL_LINEAR;
			case TextureMinifyFunction::NearestOnNearestMipmap:    return GL_NEAREST_MIPMAP_NEAREST;
			case TextureMinifyFunction::LinearOnNearestMipmap:     return GL_NEAREST;
			case TextureMinifyFunction::NearestOnLinearMipmap:     return GL_NEAREST;
			case TextureMinifyFunction::LinearOnLinearMipmap:      return GL_NEAREST;
		};
		PK_ASSERT(false, "Unknown TextureMinifyFunction, cannot determine OpenGL enum.", "Pekan");
		return 0;
	}

	unsigned RenderState::getTextureMagnifyFunctionOpenGLEnum(TextureMagnifyFunction function)
	{
		switch (function)
		{
			case TextureMagnifyFunction::Nearest:    return GL_NEAREST;
			case TextureMagnifyFunction::Linear:     return GL_LINEAR;
		};
		PK_ASSERT(false, "Unknown TextureMagnifyFunction, cannot determine OpenGL enum.", "Pekan");
		return 0;
	}

	unsigned RenderState::getTextureWrapModeOpenGLEnum(TextureWrapMode wrapMode)
	{
		switch (wrapMode)
		{
			case TextureWrapMode::ClampToEdge:       return GL_CLAMP_TO_EDGE;
			case TextureWrapMode::Repeat:            return GL_REPEAT;
			case TextureWrapMode::MirroredRepeat:    return GL_MIRRORED_REPEAT;
			case TextureWrapMode::ClampToBorder:     return GL_CLAMP_TO_BORDER;
		};
		PK_ASSERT(false, "Unknown TextureWrapMode, cannot determine OpenGL enum.", "Pekan");
		return 0;
	}

} // namespace Graphics
} // namespace Pekan