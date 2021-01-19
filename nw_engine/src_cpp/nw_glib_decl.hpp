#ifndef NW_GLIB_DECL_HPP
#define NW_GLIB_DECL_HPP

#include <nw_core.hpp>

namespace NW
{
	//	--==<control>==--
	class NW_API AGApi;
	class NW_API GEngine;
	struct NW_API GLayer;
	class NW_API GCameraLad;
	//	--==</control>==--

	//	--==<vision>==--
	class NW_API AShader;
	class NW_API GMaterial;

	//	--==</vision>==--

	//	--==<render>==--
	class NW_API ATexture;
	class NW_API ATexture1d;
	class NW_API ATexture2d;
	class NW_API ATexture3d;

	class NW_API ADrawable;
	class NW_API Particles;
	class NW_API PolyLine;
	class NW_API Polygons;
	class NW_API Triangle;
	class NW_API Rectangle;
	class NW_API Mesh3d;
	//	--==</render>==--

	//	--==<gcontext>==--
	class NW_API AWindow;
	class NW_API AGContext;
	class NW_API AFrameBuf;
	//	--==</gcontext>==--

	//	--==<gbuffers>==--
	class NW_API VertexBufLayout;
	class NW_API ShaderBufLayout;

	class NW_API AVertexBuf;
	class NW_API AIndexBuf;
	class NW_API AShaderBuf;
	//	--==</gbuffers>==--

	//	--==<structs>==--
	struct NW_API FrameBufInfo;
	struct NW_API ImageInfo;
	struct NW_API TextureInfo;
	struct NW_API RenderInfo;

	struct NW_API VertexBatch1d;
	struct NW_API VertexBatch2d;
	struct NW_API VertexShape3d;

	struct NW_API BufferElement;

	struct NW_API SubTexture1d;
	struct NW_API SubTexture2d;
	struct NW_API SubTexture3d;

	struct NW_API RenderAttribs;
	struct NW_API RenderLayer;
	
	struct NW_API GCamera;
}
namespace NW
{
	/// window_api_types
	enum WApiTypes : UInt32 {
		WAPI_NONE = 0,
		WAPI_GLFW = NW_WINDOW_GLFW
	};
	/// graphics_api_types
	/// Interface:
	/// -> Give it to the GraphicsApi create function
	/// -> Check the types for the abstracted usage
	enum GApiTypes : UInt32
	{
		GAPI_NONE = 0,
		GAPI_OPENGL = NW_GRAPHICS_OGL
	};
	enum GraphicsBufTypes : UInt32 {
		GBT_VERTEX = NW_GBUFFER_VERTEX,
		GBT_INDEX = NW_GBUFFER_INDEX,
		GBT_SHADER = NW_GBUFFER_SHADER
	};
	/// Data that can be loaded in a shader
	enum ShaderDataTypes : UInt32
	{
		SDT_NONE = NW_ERR_NO_DATA,
		SDT_BOOL = NW_BOOL, SDT_INT8 = NW_INT8, SDT_UINT8 = NW_UINT8,
		SDT_INT16 = NW_INT16, SDT_UINT16 = NW_UINT16,
		SDT_INT32 = NW_INT32, SDT_UINT32 = NW_UINT32,
		SDT_FLOAT32 = NW_FLOAT32, SDT_FLOAT64 = NW_FLOAT64,

		//SDT_FLOAT32_VEC2 = NW_FLOAT32_VEC2, SDT_FLOAT32_VEC3 = NW_FLOAT32_VEC3, SDT_FLOAT32_VEC4 = NW_FLOAT32_VEC4,
		//SDT_FLOAT32_MAT2 = NW_FLOAT32_MAT2, SDT_FLOAT32_MAT3 = NW_FLOAT32_MAT3, SDT_FLOAT32_MAT4 = NW_FLOAT32_MAT4
		
		SDT_SAMPLER = NW_SAMPLER_1D
	};
	/// Accessible (maintained) shader types
	enum ShaderTypes : UInt32
	{
		ST_SHADER = NW_SHADER,
		ST_VERTEX = NW_SHADER_VERTEX,
		ST_GEOMETRY = NW_SHADER_GEOMETRY,
		ST_PIXEL = NW_SHADER_PIXEL
	};
	enum FrameBufs : UInt32
	{
		FB_COLOR = NW_BUFFER_COLOR_BIT,
		FB_DEPTH = NW_BUFFER_DEPTH_BIT,
		FB_STENCIL = NW_BUFFER_STENCIL_BIT
	};
	enum GPrimitiveTypes : UInt32
	{
		PT_POINTS = NW_POINTS,
		PT_LINES = NW_LINES,
		PT_LINE_LOOP = NW_LINE_LOOP,
		PT_LINE_STRIP = NW_LINE_STRIP,
		PT_TRIANGLES = NW_TRIANGLES,
		PT_TRIANGLE_FAN = NW_TRIANGLE_FAN,
		PT_TRIANGLE_STRIP = NW_TRIANGLE_STRIP,
		PT_QUADS = NW_QUADS,
		PT_QUADS_STRIP = NW_QUAD_STRIP
	};
	enum DrawModes : UInt32
	{
		DM_LINE = NW_LINE,
		DM_FILL = NW_FILL
	};
	enum FacePlanes : UInt32
	{
		FP_FRONT_AND_BACK = NW_FRONT_AND_BACK,
		FP_FRONT = NW_FRONT,
		FP_BACK = NW_BACK,
		FP_RIGTH = NW_RIGHT,
		FP_LEFT = NW_LEFT,
	};
	enum ProcModes : UInt32
	{
		PM_BLEND = NW_BLEND,
		PM_DEPTH_TEST = NW_DEPTH_TEST,
		PM_STENCIL_TEST = NW_STENCIL_TEST
	};
	enum BlendConfigs : UInt32
	{
		BC_SRC_ALPHA = NW_SRC_ALPHA,
		BC_SRC_COLOR = NW_SRC_COLOR,
		BC_ONE_MINUS_SRC_ALPHA = NW_ONE_MINUS_SRC_ALPHA,
		BC_ONE_MINUS_SRC_COLOR = NW_ONE_MINUS_SRC_COLOR,
		BC_DST_ALPHA = NW_DST_ALPHA,
		BC_DST_COLOR = NW_DST_COLOR,
		BC_ONE_MUNUS_DST_ALPHA = NW_ONE_MINUS_DST_ALPHA,
		BC_ONE_MINUS_DST_COLOR = NW_ONE_MINUS_DST_COLOR,
	};
	enum DepthConfigs : UInt32
	{
		DC_EQUAL = NW_EQUAL,
		DC_LESS = NW_LESS, DC_LEQUAL = NW_LEQUAL,
		DC_GREATER = NW_GREATER, DC_GEQUAL = NW_GEQUAL,
		DC_NEVER = NW_NEVER, DC_ALWAYS = NW_ALWAYS
	};
	enum StencilConfigs : UInt32 {
		SC_KEEP = NW_KEEP, SC_ZERO = NW_ZERO, SC_REPLACE = NW_REPLACE, SC_INVERT = NW_INVERT,
		SC_INCR = NW_INCR, SC_DECR = NW_DECR
	};
	enum TextureTypes : UInt32 {
		TT_1D = NW_TEXTURE_1D, TT_2D = NW_TEXTURE_2D, TT_3D = NW_TEXTURE_3D,
	};
	enum TextureConfigs : UInt32 {
		TC_WRAP_REPEAT = NW_TEXTURE_WRAP_REPEAT, TC_WRAP_CLAMP = NW_TEXTURE_WRAP_CLAMP,
		TC_FILTER_LINEAR = NW_TEXTURE_FILTER_LINEAR, TC_FILTER_NEAREST = NW_TEXTURE_FILTER_NEAREST,
		TC_FORMAT_RED = NW_RED, TC_FORMAT_GREEN = NW_GREEN, TC_FORMAT_BLUE = NW_BLUE,
		TC_FORMAT_RGB = NW_RGB, TC_FORMAT_RGBA = NW_RGBA, TC_FORMAT_RGBA8 = NW_RGBA8,
	};
	//	--==</structs_enums>==--

	//	--==<specifications>==--
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
	class NW_API WindowOgl;
	class NW_API GContextOgl;
	class NW_API FrameBufOgl;

	class NW_API VertexArrOgl;
	class NW_API VertexBufOgl;
	class NW_API IndexBufOgl;

	class NW_API ShaderOgl;
	class NW_API ShaderProg;
	class NW_API AGMaterialOgl;
	class NW_API Texture2dOgl;
#endif	// NW_GRAPHICS
	//	--==</specifications>==--
}

//	--==<extrnal_types>==--
struct GLFWwindow;
struct GLFWimage;
//	--==</extrnal_types>==--

#define NW_MAX_TEXTURES 8
#define NW_MIN_TEXTURES 1

#define NW_MAX_DRAW_LAYERS 10
#define NW_MAX_DRAW_ORDERS 10

#endif // NW_GLIB_DECL_HPP