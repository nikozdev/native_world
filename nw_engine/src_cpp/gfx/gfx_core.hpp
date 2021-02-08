#ifndef GFX_CORE_HPP
#define GFX_CORE_HPP

// --==<linking>==--
#if defined GFX_LINK_DYNAMIC
#ifdef GFX_BUILD_DLL
#define GFX_API __declspec(dllexport)
#elif defined GFX_BUILD_EXE
#define GFX_API __declspec(dllimport)
#endif
#else
#define GFX_API
#endif
// --==</linking>==--

// --==<configurations>==--
#define GFX_GAPI_OGL		1 << 1
#define GFX_GAPI_DX		1 << 2
#if (!(defined GFX_GAPI))
#define GFX_GAPI			GFX_GAPI_DX
#endif
// --==</configurations>==--

// --debug_macro
#if (GFX_GAPI & GFX_GAPI_OGL)
#if (defined GFX_DEBUG)
// --opengl_debug
namespace NW
{
	extern inline void OglClearErr();
	extern inline bool OglErrLog(const char* funcName, const char* file, int line);
}
#define OGL_CALL(function) OglClearErr();\
		function;\
		NW_ASSERT(OglErrLog(#function, NW_FNAME_APATH((std::string)__FILE__), __LINE__), "GL_ERROR: ")
#else
#define OGL_CALL(function);
#endif // GFX_DEBUG
#endif // GFX_GAPI
#if (GFX_GAPI & GFX_GAPI_DX)
#endif // GFX_GAPI
// --==</support_macroses>==--

// --==<pragmas>==--
#pragma warning(disable : 4005)
#pragma warning(disable : 4081)
#pragma warning(disable : 4099)
#pragma warning(disable : 4102)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4552)
#pragma warning(disable : 4996)
// --==</pragmas>==--

#include <gfx_pch.hpp>

#if (GFX_GAPI & GFX_GAPI_OGL)
// --==<data_types_ogl>==--
#define GL_BOOL					0x8B56
#define GL_BYTE					0x1400
#define GL_UNSIGNED_BYTE		0x1401
#define GL_SHORT				0x1402
#define GL_UNSIGNED_SHORT		0x1403
#define GL_INT					0x1404
#define GL_UNSIGNED_INT			0x1405
#define GL_FLOAT				0x1406
#define GL_DOUBLE				0x140A
#define GL_SAMPLER_1D			0x8B5D
#define GL_UNSIGNED_INT_24_8	0x84FA
// --==</data_types_ogl>==--

// --==<data_types_glib>==--
#define GFX_ZERO			0
#define GFX_ONE			1
#define GFX_BOOL			GL_BOOL
#define GFX_INT16			GL_SHORT
#define GFX_UINT16			GL_UNSIGNED_SHORT
#define GFX_INT8			GL_BYTE
#define GFX_UINT8			GL_UNSIGNED_BYTE
#define GFX_INT32			GL_INT
#define GFX_UINT32			GL_UNSIGNED_INT
#define GFX_FLOAT32		GL_FLOAT
#define GFX_FLOAT64		GL_DOUBLE
#define GFX_FLOAT32_VEC2	(GFX_FLOAT32 + 0x0020)
#define GFX_FLOAT32_VEC3	(GFX_FLOAT32 + 0x0030)
#define GFX_FLOAT32_VEC4	(GFX_FLOAT32 + 0x0040)
#define GFX_FLOAT32_MAT2	(GFX_FLOAT32 + 0x0100)
#define GFX_FLOAT32_MAT3	(GFX_FLOAT32 + 0x0200)
#define GFX_FLOAT32_MAT4	(GFX_FLOAT32 + 0x0300)
#define GFX_SAMPLER_1D		GL_SAMPLER_1D
#define GFX_UINT24_8		GL_UNSIGNED_INT_24_8
// --==</data_types_glib>==--

// --==<draw_modes_ogl>==--
#define GL_POINT			0x1B00
#define GL_LINE				0x1B01
#define GL_FILL				0x1B02
// --primitives
#define GL_POINTS			0x0000
#define GL_LINES			0x0001
#define GL_LINE_STRIP		0x0003
#define GL_LINE_LOOP		0x0002
#define GL_TRIANGLES		0x0004
#define GL_TRIANGLE_STRIP	0x0005
#define GL_TRIANGLE_FAN		0x0006
#define GL_QUADS			0x0007
#define GL_QUAD_STRIP		0x0008
#define GL_POLYGON			0x0009
// --==</draw_modes_ogl>==--

// --==<draw_modes_glib>==--
#define GFX_POINT			0x1B00
#define GFX_LINE			0x1B01
#define GFX_FILL			0x1B02
// --primitives
#define GFX_POINTS			0x0000
#define GFX_LINES			0x0001
#define GFX_LINE_STRIP		0x0003
#define GFX_LINE_LOOP		0x0002
#define GFX_TRIANGLES		0x0004
#define GFX_TRIANGLE_STRIP	0x0005
#define GFX_TRIANGLE_FAN	0x0006
#define GFX_QUADS			0x0007
#define GFX_QUAD_STRIP		0x0008
#define GFX_POLYGON		0x0009
// --==</draw_modes_GFX>==--

// --==<planes_ogl>==--
#define GL_FRONT			0x0404
#define GL_BACK				0x0405
#define GL_LEFT				0x0406
#define GL_RIGHT			0x0407
#define GL_FRONT_AND_BACK	0x0408
// --==</planes_ogl>==--
// --==<planes_glib>==--
#define GFX_FRONT			GL_FRONT
#define GFX_BACK			GL_BACK
#define GFX_LEFT			GL_LEFT
#define GFX_RIGHT			GL_RIGHT
#define GFX_FRONT_AND_BACK	GL_FRONT_AND_BACK
// --==</planes_glib>==--

// --==<shaders_ogl>==--
#define GL_SHADER				0x82E1
#define GL_PROGRAM				0x82E2
#define GL_FRAGMENT_SHADER		0x8B30
#define GL_GEOMETRY_SHADER		0x8DD9
#define GL_VERTEX_SHADER		0x8B31
// --==<shaders_ogl>==--

// --==<shaders_glib>==--
#define GFX_SHADER				GL_PROGRAM
#define GFX_SHADER_VERTEX		GL_VERTEX_SHADER
#define GFX_SHADER_GEOMETRY	GL_GEOMETRY_SHADER
#define GFX_SHADER_PIXEL		GL_FRAGMENT_SHADER
// --==</shaders_glib>==--

// --==<configurations_ogl>==--
#define GL_POINT_SIZE			0x0B11
#define GL_LINE_SMOOTH			0x0B20
#define GL_LINE_WIDTH			0x0B21

#define GL_CULL_FACE			0x0B44
#define GL_CULL_FACE_MODE		0x0B45
#define GL_FRONT_FACE			0x0B46
#define GL_CW					0x0900
#define GL_CCW					0x0901

#define GL_DEPTH_TEST			0x0B71
#define GL_DEPTH_FUNC			0x0B74

#define GL_STENCIL_TEST			0x0B90
#define GL_STENCIL_FUNC			0x0B92

#define GL_BLEND				0x0BE2
#define GL_BLEND_DST			0x0BE0
#define GL_BLEND_SRC			0x0BE1
#define GL_SRC_COLOR			0x0300
#define GL_ONE_MINUS_SRC_COLOR	0x0301
#define GL_SRC_ALPHA			0x0302
#define GL_ONE_MINUS_SRC_ALPHA	0x0303
#define GL_DST_ALPHA			0x0304
#define GL_ONE_MINUS_DST_ALPHA	0x0305
#define GL_DST_COLOR			0x0306
#define GL_ONE_MINUS_DST_COLOR	0x0307
#define GL_SRC_ALPHA_SATURATE	0x0308

#define GL_MULTISAMPLE			0x809D

#define GL_KEEP					0x1E00
#define GL_REPLACE				0x1E01
#define GL_INCR					0x1E02
#define GL_DECR					0x1E03
// --==</configurations_ogl>==--

// --==<configurations_glib>==--
#define GFX_POINT_SIZE				GL_POINT_SIZE
#define GFX_LINE_SMOOTH			GL_LINE_SMOOTH
#define GFX_LINE_WIDTH				GL_LINE_WIDTH

#define GFX_CULL_FACE				GL_CULL_FACE
#define GFX_CULL_FACE_MODE			GL_CULL_FACE_MODE
#define GFX_FRONT_FACE				GL_FRONT_FACE
#define GFX_CW						GL_CW
#define GFX_CCW					GL_CCW

#define GFX_DEPTH_TEST				GL_DEPTH_TEST
#define GFX_DEPTH_FUNC				GL_DEPTH_FUNC

#define GFX_STENCIL_TEST			GL_STENCIL_TEST
#define GFX_STENCIL_FUNC			GL_STENCIL_FUNC

#define GFX_BLEND					GL_BLEND
#define GFX_BLEND_DST				GL_BLEND_DST
#define GFX_BLEND_SRC				GL_BLEND_SRC
#define GFX_SRC_COLOR				GL_SRC_COLOR
#define GFX_ONE_MINUS_SRC_COLOR	GL_ONE_MINUS_SRC_COLOR
#define GFX_SRC_ALPHA				GL_SRC_ALPHA
#define GFX_ONE_MINUS_SRC_ALPHA	GL_ONE_MINUS_SRC_ALPHA
#define GFX_DST_ALPHA				GL_DST_ALPHA
#define GFX_ONE_MINUS_DST_ALPHA	GL_ONE_MINUS_DST_ALPHA
#define GFX_DST_COLOR				GL_DST_COLOR
#define GFX_ONE_MINUS_DST_COLOR	GL_ONE_MINUS_DST_COLOR
#define GFX_SRC_ALPHA_SATURATE		GL_SRC_ALPHA_SATURATE

#define GFX_MULTISAMPLE			GL_MULTISAMPLE

#define GFX_KEEP					GL_KEEP
#define GFX_REPLACE				GL_REPLACE
#define GFX_INCR					GL_INCR
#define GFX_DECR					GL_DECR
// --==</configurations_glib>==--

// --==<expressions_ogl>==--
// --constants
#define GL_NONE		0
#define GL_FALSE	0
#define GL_TRUE		1
#define GL_ZERO		0
#define GL_ONE		1
// --conditions
#define GL_NEVER		0x0200
#define GL_LESS			0x0201
#define GL_EQUAL		0x0202
#define GL_LEQUAL		0x0203
#define GL_GREATER		0x0204
#define GL_NOTEQUAL		0x0205
#define GL_GEQUAL		0x0206
#define GL_ALWAYS		0x0207
// --logic
#define GL_AND				0x1501
#define GL_AND_REVERSE		0x1502
#define GL_COPY				0x1503
#define GL_AND_INVERTED		0x1504
#define GL_NOOP				0x1505
#define GL_XOR				0x1506
#define GL_OR				0x1507
#define GL_NOR				0x1508
#define GL_EQUIV			0x1509
#define GL_INVERT			0x150A
#define GL_OR_REVERSE		0x150B
#define GL_COPY_INVERTED	0x150C
#define GL_OR_INVERTED		0x150D
#define GL_NAND				0x150E
// --==</expressions_ogl>==--

// --==<expressions_glib>==--
// --constants
#define GFX_NONE		0
#define GFX_FALSE		0
#define GFX_TRUE		1
// --conditions
#define GFX_NEVER		0x0200
#define GFX_LESS		0x0201
#define GFX_EQUAL		0x0202
#define GFX_LEQUAL		0x0203
#define GFX_GREATER	0x0204
#define GFX_NOTEQUAL	0x0205
#define GFX_GEQUAL		0x0206
#define GFX_ALWAYS		0x0207
// --logic
#define GFX_AND			0x1501
#define GFX_AND_REVERSE	0x1502
#define GFX_COPY			0x1503
#define GFX_AND_INVERTED	0x1504
#define GFX_NOOP			0x1505
#define GFX_XOR			0x1506
#define GFX_OR				0x1507
#define GFX_NOR			0x1508
#define GFX_EQUIV			0x1509
#define GFX_INVERT			0x150A
#define GFX_OR_REVERSE		0x150B
#define GFX_COPY_INVERTED	0x150C
#define GFX_OR_INVERTED	0x150D
#define GFX_NAND			0x150E
// --==</expressions_glib>==--

// --==<pixel_formats_ogl>==--
#define GL_RED				0x1903
#define GL_GREEN			0x1904
#define GL_BLUE				0x1905
#define GL_ALPHA			0x1906
#define GL_RGB				0x1907
#define GL_RGBA				0x1908
#define GL_RED_INTEGER		0x8D94
#define GL_GREEN_INTEGER	0x8D95
#define GL_BLUE_INTEGER		0x8D96
#define GL_RGB_INTEGER		0x8D98
#define GL_RGBA_INTEGER		0x8D99
#define GL_R8UI				0x8232
#define GL_R16I				0x8233
#define GL_R16UI			0x8234
#define GL_R32I				0x8235
#define GL_R32UI			0x8236
#define GL_RG8I				0x8237
#define GL_RGBA8			0x8058
#define GL_DEPTH			0x1801
#define GL_STENCIL			0x1802
#define GL_DEPTH24_STENCIL8	0x88F0
// --==</pixel_formats_ogl>==--

// --==<pixel_formats_glib>==--
#define GFX_RED				GL_RED
#define GFX_GREEN				GL_GREEN
#define GFX_BLUE				GL_BLUE
#define GFX_ALPHA				GL_ALPHA
#define GFX_RGB				GL_RGB
#define GFX_RGBA				GL_RGBA
#define GFX_RED_INT			GL_RED_INTEGER
#define GFX_GREEN_INT			GL_GREEN_INTEGER
#define GFX_BLUE_INT			GL_BLUE_INTEGER
#define GFX_RGB_INT			GL_RGB_INTEGER
#define GFX_RGBA_INT			GL_RGBA_INTEGER
#define GFX_DEPTH				GL_DEPTH
#define GFX_STENCIL			GL_STENCIL
#define GFX_RED_INT32			GL_R32I
#define GFX_RED_UINT32			GL_R32UI
#define GFX_RGBA8				GL_RGBA8
#define GFX_DEPTH24			GL_DEPTH
#define GFX_STENCIL8			GL_STENCIL
#define GFX_DEPTH24_STENCIL8	GL_DEPTH24_STENCIL8
// --==</pixel_formats_glib>==--

// --==<textures_ogl>==--
#define GL_TEXTURE_1D					0x0DE0
#define GL_TEXTURE_2D					0x0DE1
#define GL_TEXTURE_3D					0x0DE2
#define GL_TEXTURE_2D_MULTISAMPLE		0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE	0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY	0x9102
#define GL_DEPTH_STENCIL				0x84F9
#define GL_DEPTH_STENCIL_ATTACHMENT		0x821A
#define GL_TEXTURE_MAG_FILTER			0x2800
#define GL_TEXTURE_MIN_FILTER			0x2801
#define GL_NEAREST						0x2600
#define GL_LINEAR						0x2601
#define GL_TEXTURE_WRAP_S				0x2802
#define GL_TEXTURE_WRAP_T				0x2803
#define GL_TEXTURE_WRAP_R				0x8072
#define GL_CLAMP						0x2900
#define GL_REPEAT						0x2901
// --==</textures_ogl>==--

// --==<textures_glib>==--
// --types
#define GFX_TEXTURE_1D					GL_TEXTURE_1D
#define GFX_TEXTURE_2D					GL_TEXTURE_2D
#define GFX_TEXTURE_3D					GL_TEXTURE_3D
#define GFX_TEXTURE_2D_MULTISAMPLE		GL_TEXTURE_2D_MULTISAMPLE
#define GFX_TEXTURE_3D_MULTISAMPLE		GL_TEXTURE_2D_MULTISAMPLE_ARRAY
#define GFX_DEPTH_STENCIL				GL_DEPTH_STENCIL
#define GFX_DEPTH_STENCIL_ATTACHMENT	GL_DEPTH_STENCIL_ATTACHMENT
// --filters
#define GFX_TEXTURE_MAG_FILTER			GL_TEXTURE_MAG_FILTER
#define GFX_TEXTURE_MIN_FILTER			GL_TEXTURE_MIN_FILTER
#define GFX_TEXTURE_FILTER_NEAREST		GL_NEAREST
#define GFX_TEXTURE_FILTER_LINEAR		GL_LINEAR
// --wrappers
#define GFX_TEXTURE_WRAP_S				GL_TEXTURE_WRAP_S
#define GFX_TEXTURE_WRAP_T				GL_TEXTURE_WRAP_T
#define GFX_TEXTURE_WRAP_R				GL_TEXTURE_WRAP_R
#define GFX_TEXTURE_WRAP_CLAMP			GL_CLAMP
#define GFX_TEXTURE_WRAP_REPEAT		GL_REPEAT
// --==</textures_glib>==--

// --==<gbuffers_ogl>==--
#define GL_ARRAY_BUFFER			0x8892
#define GL_ELEMENT_ARRAY_BUFFER	0x8893
#define GL_UNIFORM_BUFFER		0x8A11
// --==</gbuffers_ogl>==--


// --==<gbuffers_glib>==--
#define GFX_GBUFFER_VERTEX		GL_ARRAY_BUFFER
#define GFX_GBUFFER_INDEX		GL_ELEMENT_ARRAY_BUFFER
#define GFX_GBUFFER_SHADER		GL_UNIFORM_BUFFER
// --==</gbuffers_glib>==--

// --==<framebuffers_ogl>==--
#define GL_DRAW_BUFFER			0x0C01
#define GL_READ_BUFFER			0x0C02
#define GL_FRAMEBUFFER			0x8D40
#define GL_COLOR_BUFFER_BIT		0x00004000
#define GL_DEPTH_BUFFER_BIT		0x00000100
#define GL_ACCUM_BUFFER_BIT		0x00000200
#define GL_STENCIL_BUFFER_BIT	0x00000400
#define GL_DEPTH_ATTACHMENT		0x8D00
#define GL_STENCIL_ATTACHMENT	0x8D20
// --==</buffers_ogl>==--

// --==<framebuffers_glib>==--
#define GFX_FRAMEBUF_IN		GL_DRAW_BUFFER
#define GFX_FRAMEBUF_OUT		GL_READ_BUFFER
#define GFX_FRAMEBUF_IN_OUT	GL_FRAMEBUFFER
#define GFX_BUFFER_COLOR_BIT	GL_COLOR_BUFFER_BIT
#define GFX_BUFFER_DEPTH_BIT	GL_DEPTH_BUFFER_BIT
#define GFX_BUFFER_STENCIL_BIT	GL_STENCIL_BUFFER_BIT
// --==</framebuffers_glib>==--

// --==<data_load_types_ogl>==--
#define GL_STATIC_DRAW			0x88E4
#define GL_STREAM_DRAW			0x88E0
#define GL_DYNAMIC_DRAW			0x88E8
// --==</data_load_types_ogl>==--

// --==<data_load_types_glib>==--
#define GFX_STATIC_DRAW		GL_STATIC_DRAW 
#define GFX_STREAM_DRAW		GL_STREAM_DRAW 
#define GFX_DYNAMIC_DRAW		GL_DYNAMIC_DRAW
// --==</data_load_types_glib>==--

// --==<error_codes_ogl>==--
#define GL_NO_ERROR				0
#define GL_INVALID_ENUM			0x0500
#define GL_INVALID_VALUE		0x0501
#define GL_INVALID_OPERATION	0x0502
#define GL_STACK_OVERFLOW		0x0503
#define GL_STACK_UNDERFLOW		0x0504
#define GL_OUT_OF_MEMORY		0x0505
// --==</error_codes_ogl>==--

// --==<error_codes_glib>==--
#define GFX_OK					GL_NO_ERROR         
#define GFX_INVALID_ENUM		GL_INVALID_ENUM     
#define GFX_INVALID_VALUE		GL_INVALID_VALUE    
#define GFX_INVALID_OPERATION	GL_INVALID_OPERATION
#define GFX_STACK_OVERFLOW		GL_STACK_OVERFLOW   
#define GFX_STACK_UNDERFLOW	GL_STACK_UNDERFLOW  
#define GFX_OUT_OF_MEMORY		GL_OUT_OF_MEMORY  
// --==</error_codes_glib>==--

// --==<info_ogl>==--
#define GL_VENDOR				0x1F00
#define GL_RENDERER				0x1F01
#define GL_VERSION				0x1F02
#define GL_EXTENSIONS			0x1F03

#define GL_COMPILE_STATUS		0x8B81
#define GL_LINK_STATUS			0x8B82
#define GL_INFO_LOG_LENGTH		0x8B84
#define GL_ATTACHED_SHADERS		0x8B85
// --==</info_ogl>==--

// --==<info_glib>==--
#define GFX_COMPILE_STATUS		GL_COMPILE_STATUS
#define GFX_LINK_STATUS		GL_LINK_STATUS
// --==</info_glib>==--
#endif // GFX_GAPI
#if (GFX_GAPI & GFX_GAPI_DX)
// --==<data_types_dx11>==--
// --==</data_types_dx11>==--

// --==<data_types_glib>==--
#define GFX_ZERO			0
#define GFX_ONE			1
#define GFX_BOOL			2
#define GFX_INT16			3
#define GFX_UINT16			4
#define GFX_INT8			5
#define GFX_UINT8			6
#define GFX_INT32			7
#define GFX_UINT32			8
#define GFX_FLOAT32		9
#define GFX_FLOAT64		10
#define GFX_FLOAT32_VEC2	11
#define GFX_FLOAT32_VEC3	12
#define GFX_FLOAT32_VEC4	13
#define GFX_FLOAT32_MAT2	14
#define GFX_FLOAT32_MAT3	15
#define GFX_FLOAT32_MAT4	16
#define GFX_SAMPLER_1D		17
#define GFX_UINT24_8		18
// --==</data_types_glib>==--

// --==<draw_modes_dx11>==--
// --primitives
// --==</draw_modes_dx11>==--

// --==<draw_modes_glib>==--
#define GFX_POINT	0
#define GFX_LINE	0
#define GFX_FILL	0
// --primitives
#define GFX_POINTS			0
#define GFX_LINES			0
#define GFX_LINE_STRIP		0
#define GFX_LINE_LOOP		0
#define GFX_LINE_BIT		0
#define GFX_TRIANGLES		0
#define GFX_TRIANGLE_STRIP 0
#define GFX_TRIANGLE_FAN	0
#define GFX_QUADS			0
#define GFX_QUAD_STRIP		0
#define GFX_POLYGON		0
// --==</draw_modes_glib>==--

// --==<planes_dx11>==--
// --==</planes_dx11>==--

// --==<planes_glib>==--
#define GFX_FRONT			0
#define GFX_BACK			0
#define GFX_LEFT			0
#define GFX_RIGHT			0
#define GFX_FRONT_AND_BACK	0
// --==</planes_glib>==--

// --==<shaders_dx11>==--
// --==<shaders_dx11>==--

// --==<shaders_glib>==--
#define GFX_SHADER					0
#define GFX_SHADER_VERTEX			0
#define GFX_SHADER_GEOMETRY		0
#define GFX_SHADER_PIXEL			0
// --==</shaders_glib>==--

// --==<configurations_dx11>==--
// --==</configurations_dx11>==--

// --==<configurations_glib>==--
#define GFX_POINT_SIZE				0
#define GFX_LINE_SMOOTH			0
#define GFX_LINE_WIDTH				0

#define GFX_CULL_FACE				0
#define GFX_CULL_FACE_MODE			0
#define GFX_FRONT_FACE				0
#define GFX_CW						0
#define GFX_CCW					0

#define GFX_DEPTH_TEST				0
#define GFX_DEPTH_FUNC				0

#define GFX_STENCIL_TEST			0
#define GFX_STENCIL_FUNC			0

#define GFX_BLEND					0
#define GFX_BLEND_DST				0
#define GFX_BLEND_SRC				0
#define GFX_SRC_COLOR				0
#define GFX_ONE_MINUS_SRC_COLOR	0
#define GFX_SRC_ALPHA				0
#define GFX_ONE_MINUS_SRC_ALPHA	0
#define GFX_DST_ALPHA				0
#define GFX_ONE_MINUS_DST_ALPHA	0
#define GFX_DST_COLOR				0
#define GFX_ONE_MINUS_DST_COLOR	0
#define GFX_SRC_ALPHA_SATURATE		0

#define GFX_MULTISAMPLE			0

#define GFX_KEEP					0
#define GFX_REPLACE				0
#define GFX_INCR					0
#define GFX_DECR					0
// --==</configurations_glib>==--

// --==<expressions_dx11>==--
// --constants
// --conditions
// --logic
// --==</expressions_dx11>==--

// --==<expressions_glib>==--
// --constants
#define GFX_NONE		0
#define GFX_FALSE		0
#define GFX_TRUE		1
// --conditions
#define GFX_NEVER		0x0200
#define GFX_LESS		0x0201
#define GFX_EQUAL		0x0202
#define GFX_LEQUAL		0x0203
#define GFX_GREATER	0x0204
#define GFX_NOTEQUAL	0x0205
#define GFX_GEQUAL		0x0206
#define GFX_ALWAYS		0x0207
// --logic
#define GFX_AND			0x1501
#define GFX_AND_REVERSE	0x1502
#define GFX_COPY			0x1503
#define GFX_AND_INVERTED	0x1504
#define GFX_NOOP			0x1505
#define GFX_XOR			0x1506
#define GFX_OR				0x1507
#define GFX_NOR			0x1508
#define GFX_EQUIV			0x1509
#define GFX_INVERT			0x150A
#define GFX_OR_REVERSE		0x150B
#define GFX_COPY_INVERTED	0x150C
#define GFX_OR_INVERTED	0x150D
#define GFX_NAND			0x150E
// --==</expressions_glib>==--

// --==<pixel_formats_dx11>==--
// --==</pixel_formats_dx11>==--

// --==<pixel_formats_glib>==--
#define GFX_RED				0
#define GFX_GREEN				0
#define GFX_BLUE				0
#define GFX_ALPHA				0
#define GFX_RGB				0
#define GFX_RGBA				0
#define GFX_RED_INT			0
#define GFX_GREEN_INT			0
#define GFX_BLUE_INT			0
#define GFX_RGB_INT			0
#define GFX_RGBA_INT			0
#define GFX_DEPTH				0
#define GFX_STENCIL			0
#define GFX_RED_INT32			0
#define GFX_RED_UINT32			0
#define GFX_RGBA8				0
#define GFX_DEPTH24			0
#define GFX_STENCIL8			0
#define GFX_DEPTH24_STENCIL8	0
// --==</pixel_formats_glib>==--

// --==<textures_dx11>==--
// --==</textures_dx11>==--

// --==<textures_glib>==--
// --types
#define GFX_TEXTURE_1D					1
#define GFX_TEXTURE_2D					2
#define GFX_TEXTURE_3D					3
#define GFX_TEXTURE_2D_MULTISAMPLE		4
#define GFX_TEXTURE_3D_MULTISAMPLE		5
#define GFX_DEPTH_STENCIL				6
#define GFX_DEPTH_STENCIL_ATTACHMENT	7
// --filters
#define GFX_TEXTURE_MAG_FILTER			0
#define GFX_TEXTURE_MIN_FILTER			0
#define GFX_TEXTURE_FILTER_NEAREST		0
#define GFX_TEXTURE_FILTER_LINEAR		0
// --wrappers
#define GFX_TEXTURE_WRAP_S				0
#define GFX_TEXTURE_WRAP_T				0
#define GFX_TEXTURE_WRAP_R				0
#define GFX_TEXTURE_WRAP_CLAMP			0
#define GFX_TEXTURE_WRAP_REPEAT		0
// --==</textures_glib>==--

// --==<gbuffers_dx11>==--
// --==</gbuffers_dx11>==--


// --==<gbuffers_glib>==--
#define GFX_GBUFFER_VERTEX	0
#define GFX_GBUFFER_INDEX	0
#define GFX_GBUFFER_SHADER	0
// --==</gbuffers_glib>==--

// --==<framebuffers_dx11>==--
// --==</framebuffers_dx11>==--

// --==<framebuffers_glib>==--
#define GFX_FRAMEBUF_IN		0
#define GFX_FRAMEBUF_OUT		0
#define GFX_FRAMEBUF_IN_OUT	0
#define GFX_BUFFER_COLOR_BIT	0
#define GFX_BUFFER_DEPTH_BIT	0
#define GFX_BUFFER_STENCIL_BIT	0
// --==</framebuffers_glib>==--

// --==<data_load_types_dx11>==--
// --==</data_load_types_dx11>==--

// --==<data_load_types_glib>==--
#define GFX_STATIC_DRAW		0
#define GFX_STREAM_DRAW		0
#define GFX_DYNAMIC_DRAW		0
// --==</data_load_types_glib>==--

// --==<error_codes_dx11>==--
// --==</error_codes_dx11>==--

// --==<error_codes_glib>==--
#define GFX_OK					0
#define GFX_INVALID_ENUM		0
#define GFX_INVALID_VALUE		0
#define GFX_INVALID_OPERATION	0
#define GFX_STACK_OVERFLOW		0
#define GFX_STACK_UNDERFLOW	0
#define GFX_OUT_OF_MEMORY		0
// --==</error_codes_glib>==--

// --==<info_dx11>==--
// --==</info_dx11>==--

// --==<info_glib>==--
#define GFX_COMPILE_STATUS			0
#define GFX_LINK_STATUS			0
// --==</info_glib>==--
#endif	// GFX_GAPI

#if (defined GFX_GAPI)
namespace NW
{
#if (defined GFX_PLATFORM_WINDOWS)
	using GLibModule = HMODULE;
	using GLibProc = FARPROC;
#else
	using GLibModule = Ptr;
	using GLibProc = Ptr;
#endif
	#if (GFX_GAPI & GFX_GAPI_OGL)
	typedef GLibProc(*GLibLoader)(const char* strName);
	#endif
	#if (GFX_GAPI & GFX_GAPI_DX)
	typedef GLibProc(*GlibLoader)(const char* strName);
	#endif
}
#endif	// GFX_GAPI

namespace NW
{
	class GFX_API GfxEngine;

	class GFX_API Shader;
	class GFX_API GMaterial;
	class GFX_API Texture;

	class GFX_API FrameBuf;

	class GFX_API VertexBufLayout;
	class GFX_API ShaderBufLayout;

	class GFX_API VertexBuf;
	class GFX_API IndexBuf;
	class GFX_API ShaderBuf;
	class GFX_API VertexArr;

	class GFX_API GCameraLad;
}
namespace NW
{
	struct GFX_API Drawable;

	struct GFX_API GCamera;

	struct GFX_API FrameBufInfo;
	struct GFX_API TextureInfo;

	struct GFX_API VertexBatch1d;
	struct GFX_API VertexBatch2d;
	struct GFX_API VertexShape3d;

	struct GFX_API BufferElement;

	struct GFX_API SubTexture1d;
	struct GFX_API SubTexture2d;
	struct GFX_API SubTexture3d;
}
namespace NW
{
	/// graphics_api_types
	/// Interface:
	/// -> Give it to the GraphicsApi create function
	/// -> Check the types for the abstracted usage
	enum GApiTypes : UInt32 {
		GAPI_DEFAULT = GFX_GAPI_OGL,
		GAPI_OPENGL = GFX_GAPI_OGL,
		GAPI_DIRECTX = GFX_GAPI_DX,
	};
	enum GBufferTypes : UInt32 {
		GBT_DEFAULT = GFX_GBUFFER_VERTEX,
		GBT_VERTEX = GFX_GBUFFER_VERTEX,
		GBT_INDEX = GFX_GBUFFER_INDEX,
		GBT_SHADER = GFX_GBUFFER_SHADER,
	};
	enum FrameBufTypes : UInt32 {
		FBT_DEFAULT = GFX_FRAMEBUF_IN_OUT,
		FBT_IN = GFX_FRAMEBUF_IN, FBT_OUT = GFX_FRAMEBUF_OUT, FBT_IN_OUT = GFX_FRAMEBUF_IN_OUT,
	};
	/// Data that can be loaded in a shader
	enum ShaderDataTypes : UInt32
	{
		SDT_DEFAULT = GFX_FLOAT32,
		SDT_BOOL = GFX_BOOL, SDT_INT8 = GFX_INT8, SDT_UINT8 = GFX_UINT8,
		SDT_INT16 = GFX_INT16, SDT_UINT16 = GFX_UINT16,
		SDT_INT32 = GFX_INT32, SDT_UINT32 = GFX_UINT32,
		SDT_FLOAT32 = GFX_FLOAT32, SDT_FLOAT64 = GFX_FLOAT64,

		SDT_SAMPLER = GFX_SAMPLER_1D
	};
	/// Accessible (maintained) shader types
	enum ShaderTypes : UInt32
	{
		ST_DEFAULT = GFX_SHADER,
		ST_SHADER = GFX_SHADER,
		ST_VERTEX = GFX_SHADER_VERTEX,
		ST_GEOMETRY = GFX_SHADER_GEOMETRY,
		ST_PIXEL = GFX_SHADER_PIXEL
	};
	enum FrameBufs : UInt32
	{
		FB_DEFAULT = GFX_BUFFER_COLOR_BIT | GFX_BUFFER_DEPTH_BIT | GFX_BUFFER_STENCIL_BIT,
		FB_COLOR = GFX_BUFFER_COLOR_BIT,
		FB_DEPTH = GFX_BUFFER_DEPTH_BIT,
		FB_STENCIL = GFX_BUFFER_STENCIL_BIT
	};
	enum GPrimitiveTypes : UInt32
	{
		PT_DEFAULT = GFX_TRIANGLES,
		PT_POINTS = GFX_POINTS,
		PT_LINES = GFX_LINES,
		PT_LINE_LOOP = GFX_LINE_LOOP,
		PT_LINE_STRIP = GFX_LINE_STRIP,
		PT_TRIANGLES = GFX_TRIANGLES,
		PT_TRIANGLE_FAN = GFX_TRIANGLE_FAN,
		PT_TRIANGLE_STRIP = GFX_TRIANGLE_STRIP,
		PT_QUADS = GFX_QUADS,
		PT_QUADS_STRIP = GFX_QUAD_STRIP
	};
	enum DrawModes : UInt32 {
		DM_DEFAULT = GFX_FILL,
		DM_LINE = GFX_LINE, DM_FILL = GFX_FILL
	};
	enum FacePlanes : UInt32 {
		FACE_DEFAULT = GFX_FRONT_AND_BACK,
		FACE_FRONT_AND_BACK = GFX_FRONT_AND_BACK,
		FACE_BACK = GFX_BACK, FACE_FRONT = GFX_FRONT,
		FACE_LEFT = GFX_LEFT, FACE_RIGTH = GFX_RIGHT,
	};
	enum ProcessingModes : UInt32 {
		PM_BLEND = GFX_BLEND,
		PM_MULTISAMPLE = GFX_MULTISAMPLE,
		PM_CULL_FACE = GFX_CULL_FACE,
		PM_DEPTH_TEST = GFX_DEPTH_TEST,
		PM_STENCIL_TEST = GFX_STENCIL_TEST
	};
	enum BlendConfigs : UInt32 {
		BC_SRC_DEFAULT = 0, BC_DEST_DEFAULT = 0,
		BC_SRC_ALPHA = GFX_SRC_ALPHA, BC_SRC_COLOR = GFX_SRC_COLOR,
		BC_ONE_MINUS_SRC_ALPHA = GFX_ONE_MINUS_SRC_ALPHA,
		BC_ONE_MINUS_SRC_COLOR = GFX_ONE_MINUS_SRC_COLOR,
		BC_DST_ALPHA = GFX_DST_ALPHA, BC_DST_COLOR = GFX_DST_COLOR,
		BC_ONE_MUNUS_DST_ALPHA = GFX_ONE_MINUS_DST_ALPHA,
		BC_ONE_MINUS_DST_COLOR = GFX_ONE_MINUS_DST_COLOR,
	};
	enum CullFaceConfigs : UInt32 {
		CFC_DEFAULT = 0
	};
	enum DepthConfigs : UInt32 {
		DTC_DEFAULT = GFX_GREATER,
		DTC_EQUAL = GFX_EQUAL,
		DTC_LESS = GFX_LESS, DTC_LEQUAL = GFX_LEQUAL,
		DTC_GREATER = GFX_GREATER, DTC_GEQUAL = GFX_GEQUAL,
		DTC_NEVER = GFX_NEVER, DTC_ALWAYS = GFX_ALWAYS
	};
	enum StencilConfigs : UInt32 {
		STC_DEFAULT = 0,
		STC_KEEP = GFX_KEEP, STC_ZERO = GFX_ZERO, STC_REPLACE = GFX_REPLACE, STC_INVERT = GFX_INVERT,
		STC_INCR = GFX_INCR, STC_DECR = GFX_DECR
	};
	enum TextureTypes : UInt32 {
		TXT_NONE = 0,
		TXT_1D = GFX_TEXTURE_1D, TXT_2D = GFX_TEXTURE_2D, TXT_3D = GFX_TEXTURE_3D,
		TXT_2D_MULTISAMPLE = GFX_TEXTURE_2D_MULTISAMPLE, TXT_3D_MULTISAMPLE = GFX_TEXTURE_3D_MULTISAMPLE
	};
	enum TextureWraps : UInt32 {
		TXW_NONE = 0, TXW_REPEAT = GFX_TEXTURE_WRAP_REPEAT, TXW_CLAMP = GFX_TEXTURE_WRAP_CLAMP,
	};
	enum TextureFilters : UInt32 {
		TXF_LINEAR = GFX_TEXTURE_FILTER_LINEAR, TXF_NEAREST = GFX_TEXTURE_FILTER_NEAREST,
	};
	enum TextureFormats : UInt32 {
		TXF_NONE = 0,
		TXF_RED = GFX_RED, TXF_GREEN = GFX_GREEN, TXF_BLUE = GFX_BLUE,
		TXF_RGB = GFX_RGB, TXF_RGBA = GFX_RGBA,
		TXF_RED_INT = GFX_RED_INT, TXF_GREEN_INT = GFX_GREEN_INT, TXF_BLUE_INT = GFX_BLUE_INT,
		TXF_RGB_INT = GFX_RGB_INT, TXF_RGBA_INT = GFX_RGBA_INT,
		TXF_DEPTH = GFX_DEPTH, TXF_STENCIL = GFX_STENCIL,
		TXF_DEPTH_STENCIL = GFX_DEPTH_STENCIL,
	};
	enum TextureInterFormats : UInt32 {
		TXFI_RED_UINT32 = GFX_RED_UINT32, TXFI_RED_INT32 = GFX_RED_INT32,
		TXFI_RGB = GFX_RGB, TXFI_RGBA8 = GFX_RGBA8,
		TXFI_DEPTH24 = GFX_DEPTH24, TXFI_STENCIL8 = GFX_STENCIL8,
		TXFI_DEPTH24_STENCIL8 = GFX_DEPTH24_STENCIL8,
	};
	enum PixelFormats : UInt32 {
		FBAT_DEFAULT = 0,
		PXF_INT8 = GFX_INT8, PXF_UINT8 = GFX_UINT8,
		PXF_INT32 = GFX_INT32, PXF_UINT32 = GFX_UINT32,
		PXF_UINT24_8 = GFX_UINT24_8
	};
	//	--==</structs_enums>==--
}

#endif	// GFX_CORE_HPP

/*
* The project created in 16.01.2021
* Graphics library as a tool for game engines and applications
*/