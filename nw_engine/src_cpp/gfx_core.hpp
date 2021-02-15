#ifndef NWG_CORE_HPP
#define NWG_CORE_HPP

#include <nw_core.hpp>

#if (NW_GAPI & NW_GAPI_OGL)
#if (defined NW_DEBUG)
// --opengl_debug
namespace NW
{
	extern inline void OglClearErr();
	extern inline bool OglErrLog(const char* funcName, const char* file, int line);
}
#define NW_OGL_CALL(function) OglClearErr();\
		function;\
		NWL_ASSERT(OglErrLog(#function, NWL_FNAME_APATH((std::string)__FILE__), __LINE__), "GL_ERROR: ")
#else
	#define NW_OGL_CALL(function);
#endif // NW_DEBUG
#endif // NW_GAPI
#if (NW_GAPI & NW_GAPI_DX)
#endif // NW_GAPI

#if (NW_GAPI & NW_GAPI_OGL)
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

// --==<data_types_nw>==--
#define NW_ZERO			0
#define NW_ONE			1
#define NW_BOOL			GL_BOOL
#define NW_INT16		GL_SHORT
#define NW_UINT16		GL_UNSIGNED_SHORT
#define NW_INT8			GL_BYTE
#define NW_UINT8		GL_UNSIGNED_BYTE
#define NW_INT32		GL_INT
#define NW_UINT32		GL_UNSIGNED_INT
#define NW_FLOAT32		GL_FLOAT
#define NW_FLOAT64		GL_DOUBLE
#define NW_FLOAT32_VEC2	(NW_FLOAT32 + 0x0020)
#define NW_FLOAT32_VEC3	(NW_FLOAT32 + 0x0030)
#define NW_FLOAT32_VEC4	(NW_FLOAT32 + 0x0040)
#define NW_FLOAT32_MAT2	(NW_FLOAT32 + 0x0100)
#define NW_FLOAT32_MAT3	(NW_FLOAT32 + 0x0200)
#define NW_FLOAT32_MAT4	(NW_FLOAT32 + 0x0300)
#define NW_SAMPLER_1D	GL_SAMPLER_1D
#define NW_UINT24_8		GL_UNSIGNED_INT_24_8
// --==</data_types_nw>==--

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
// --==</draw_modes_ogl>==--

// --==<draw_modes_nw>==--
#define NW_POINT			0x1B00
#define NW_LINE				0x1B01
#define NW_FILL				0x1B02
// --primitives
#define NW_POINTS			0x0000
#define NW_LINES			0x0001
#define NW_LINE_STRIP		0x0003
#define NW_LINE_LOOP		0x0002
#define NW_TRIANGLES		0x0004
#define NW_TRIANGLE_STRIP	0x0005
#define NW_TRIANGLE_FAN		0x0006
// --==</draw_modes_GFX>==--

// --==<planes_ogl>==--
#define GL_FRONT			0x0404
#define GL_BACK				0x0405
#define GL_LEFT				0x0406
#define GL_RIGHT			0x0407
#define GL_FRONT_AND_BACK	0x0408
// --==</planes_ogl>==--
// --==<planes_nw>==--
#define NW_FRONT			GL_FRONT
#define NW_BACK				GL_BACK
#define NW_LEFT				GL_LEFT
#define NW_RIGHT			GL_RIGHT
#define NW_FRONT_AND_BACK	GL_FRONT_AND_BACK
// --==</planes_nw>==--

// --==<shaders_ogl>==--
#define GL_SHADER				0x82E1
#define GL_PROGRAM				0x82E2
#define GL_FRAGMENT_SHADER		0x8B30
#define GL_GEOMETRY_SHADER		0x8DD9
#define GL_VERTEX_SHADER		0x8B31
// --==<shaders_ogl>==--

// --==<shaders_nw>==--
#define NW_SHADER				GL_PROGRAM
#define NW_SHADER_VERTEX		GL_VERTEX_SHADER
#define NW_SHADER_GEOMETRY		GL_GEOMETRY_SHADER
#define NW_SHADER_PIXEL			GL_FRAGMENT_SHADER
// --==</shaders_nw>==--

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

// --==<configurations_nw>==--
#define NW_POINT_SIZE				GL_POINT_SIZE
#define NW_LINE_SMOOTH				GL_LINE_SMOOTH
#define NW_LINE_WIDTH				GL_LINE_WIDTH

#define NW_CULL_FACE				GL_CULL_FACE
#define NW_CULL_FACE_MODE			GL_CULL_FACE_MODE
#define NW_FRONT_FACE				GL_FRONT_FACE
#define NW_CW						GL_CW
#define NW_CCW						GL_CCW

#define NW_DEPTH_TEST				GL_DEPTH_TEST
#define NW_DEPTH_FUNC				GL_DEPTH_FUNC

#define NW_STENCIL_TEST			GL_STENCIL_TEST
#define NW_STENCIL_FUNC			GL_STENCIL_FUNC

#define NW_BLEND					GL_BLEND
#define NW_BLEND_DST				GL_BLEND_DST
#define NW_BLEND_SRC				GL_BLEND_SRC
#define NW_SRC_COLOR				GL_SRC_COLOR
#define NW_ONE_MINUS_SRC_COLOR		GL_ONE_MINUS_SRC_COLOR
#define NW_SRC_ALPHA				GL_SRC_ALPHA
#define NW_ONE_MINUS_SRC_ALPHA		GL_ONE_MINUS_SRC_ALPHA
#define NW_DST_ALPHA				GL_DST_ALPHA
#define NW_ONE_MINUS_DST_ALPHA		GL_ONE_MINUS_DST_ALPHA
#define NW_DST_COLOR				GL_DST_COLOR
#define NW_ONE_MINUS_DST_COLOR		GL_ONE_MINUS_DST_COLOR
#define NW_SRC_ALPHA_SATURATE		GL_SRC_ALPHA_SATURATE

#define NW_MULTISAMPLE			GL_MULTISAMPLE

#define NW_KEEP					GL_KEEP
#define NW_REPLACE				GL_REPLACE
#define NW_INCR					GL_INCR
#define NW_DECR					GL_DECR
// --==</configurations_nw>==--

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

// --==<expressions_nw>==--
// --constants
#define NW_NONE			0
#define NW_FALSE		0
#define NW_TRUE			1
// --conditions
#define NW_NEVER		0x0200
#define NW_LESS			0x0201
#define NW_EQUAL		0x0202
#define NW_LEQUAL		0x0203
#define NW_GREATER		0x0204
#define NW_NOTEQUAL		0x0205
#define NW_GEQUAL		0x0206
#define NW_ALWAYS		0x0207
// --logic
#define NW_AND				0x1501
#define NW_AND_REVERSE		0x1502
#define NW_COPY				0x1503
#define NW_AND_INVERTED		0x1504
#define NW_NOOP				0x1505
#define NW_XOR				0x1506
#define NW_OR				0x1507
#define NW_NOR				0x1508
#define NW_EQUIV			0x1509
#define NW_INVERT			0x150A
#define NW_OR_REVERSE		0x150B
#define NW_COPY_INVERTED	0x150C
#define NW_OR_INVERTED		0x150D
#define NW_NAND				0x150E
// --==</expressions_nw>==--

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

// --==<pixel_formats_nw>==--
#define NW_RED				GL_RED
#define NW_GREEN			GL_GREEN
#define NW_BLUE				GL_BLUE
#define NW_ALPHA			GL_ALPHA
#define NW_RGB				GL_RGB
#define NW_RGBA				GL_RGBA
#define NW_RED_INT			GL_RED_INTEGER
#define NW_GREEN_INT		GL_GREEN_INTEGER
#define NW_BLUE_INT			GL_BLUE_INTEGER
#define NW_RGB_INT			GL_RGB_INTEGER
#define NW_RGBA_INT			GL_RGBA_INTEGER
#define NW_DEPTH			GL_DEPTH
#define NW_STENCIL			GL_STENCIL
#define NW_RED_INT32		GL_R32I
#define NW_RED_UINT32		GL_R32UI
#define NW_RGBA8			GL_RGBA8
#define NW_DEPTH24			GL_DEPTH
#define NW_STENCIL8			GL_STENCIL
#define NW_DEPTH24_STENCIL8	GL_DEPTH24_STENCIL8
// --==</pixel_formats_nw>==--

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

// --==<textures_nw>==--
// --types
#define NW_TEXTURE_1D					GL_TEXTURE_1D
#define NW_TEXTURE_2D					GL_TEXTURE_2D
#define NW_TEXTURE_3D					GL_TEXTURE_3D
#define NW_TEXTURE_2D_MULTISAMPLE		GL_TEXTURE_2D_MULTISAMPLE
#define NW_TEXTURE_3D_MULTISAMPLE		GL_TEXTURE_2D_MULTISAMPLE_ARRAY
#define NW_DEPTH_STENCIL				GL_DEPTH_STENCIL
#define NW_DEPTH_STENCIL_ATTACHMENT		GL_DEPTH_STENCIL_ATTACHMENT
// --filters
#define NW_TEXTURE_MAG_FILTER			GL_TEXTURE_MAG_FILTER
#define NW_TEXTURE_MIN_FILTER			GL_TEXTURE_MIN_FILTER
#define NW_TEXTURE_FILTER_NEAREST		GL_NEAREST
#define NW_TEXTURE_FILTER_LINEAR		GL_LINEAR
// --wrappers
#define NW_TEXTURE_WRAP_S				GL_TEXTURE_WRAP_S
#define NW_TEXTURE_WRAP_T				GL_TEXTURE_WRAP_T
#define NW_TEXTURE_WRAP_R				GL_TEXTURE_WRAP_R
#define NW_TEXTURE_WRAP_CLAMP			GL_CLAMP
#define NW_TEXTURE_WRAP_REPEAT			GL_REPEAT
// --==</textures_nw>==--

// --==<gbuffers_ogl>==--
#define GL_ARRAY_BUFFER				0x8892
#define GL_ELEMENT_ARRAY_BUFFER		0x8893
#define GL_UNIFORM_BUFFER			0x8A11
// --==</gbuffers_ogl>==--


// --==<gbuffers_nw>==--
#define NWG_BUFFER_VERTEX		GL_ARRAY_BUFFER
#define NWG_BUFFER_INDEX			GL_ELEMENT_ARRAY_BUFFER
#define NWG_BUFFER_SHADER		GL_UNIFORM_BUFFER
// --==</gbuffers_nw>==--

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

// --==<framebuffers_nw>==--
#define NW_FRAMEBUF_IN			GL_DRAW_BUFFER
#define NW_FRAMEBUF_OUT			GL_READ_BUFFER
#define NW_FRAMEBUF_IN_OUT		GL_FRAMEBUFFER
#define NW_BUFFER_COLOR_BIT		GL_COLOR_BUFFER_BIT
#define NW_BUFFER_DEPTH_BIT		GL_DEPTH_BUFFER_BIT
#define NW_BUFFER_STENCIL_BIT	GL_STENCIL_BUFFER_BIT
// --==</framebuffers_nw>==--

// --==<data_load_types_ogl>==--
#define GL_STATIC_DRAW			0x88E4
#define GL_STREAM_DRAW			0x88E0
#define GL_DYNAMIC_DRAW			0x88E8
// --==</data_load_types_ogl>==--

// --==<data_load_types_nw>==--
#define NW_STATIC_DRAW		GL_STATIC_DRAW 
#define NW_STREAM_DRAW		GL_STREAM_DRAW 
#define NW_DYNAMIC_DRAW		GL_DYNAMIC_DRAW
// --==</data_load_types_nw>==--

// --==<error_codes_ogl>==--
#define GL_NO_ERROR				0
#define GL_INVALID_ENUM			0x0500
#define GL_INVALID_VALUE		0x0501
#define GL_INVALID_OPERATION	0x0502
#define GL_STACK_OVERFLOW		0x0503
#define GL_STACK_UNDERFLOW		0x0504
#define GL_OUT_OF_MEMORY		0x0505
// --==</error_codes_ogl>==--

// --==<error_codes_nw>==--
#define NW_OK					GL_NO_ERROR         
#define NW_INVALID_ENUM			GL_INVALID_ENUM     
#define NW_INVALID_VALUE		GL_INVALID_VALUE    
#define NW_INVALID_OPERATION	GL_INVALID_OPERATION
#define NW_STACK_OVERFLOW		GL_STACK_OVERFLOW   
#define NW_STACK_UNDERFLOW		GL_STACK_UNDERFLOW  
#define NW_OUT_OF_MEMORY		GL_OUT_OF_MEMORY  
// --==</error_codes_nw>==--

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

// --==<info_nw>==--
#define NW_COMPILE_STATUS		GL_COMPILE_STATUS
#define NW_LINK_STATUS			GL_LINK_STATUS
// --==</info_nw>==--
#endif // NW_GAPI
#if (NW_GAPI & NW_GAPI_DX)
// --==<data_types_dx11>==--
// --==</data_types_dx11>==--

// --==<data_types_nw>==--
#define NW_ZERO			0
#define NW_ONE			1
#define NW_BOOL			2
#define NW_INT16		3
#define NW_UINT16		4
#define NW_INT8			5
#define NW_UINT8		6
#define NW_INT32		7
#define NW_UINT32		8
#define NW_FLOAT32		9
#define NW_FLOAT64		10
#define NW_FLOAT32_VEC2	11
#define NW_FLOAT32_VEC3	12
#define NW_FLOAT32_VEC4	13
#define NW_FLOAT32_MAT2	14
#define NW_FLOAT32_MAT3	15
#define NW_FLOAT32_MAT4	16
#define NW_SAMPLER_1D	17
#define NW_UINT24_8		18
// --==</data_types_nw>==--

// --==<draw_modes_dx11>==--
// --primitives
#define	D3D_PT_UNDEFINED			0
#define	D3D_PT_POINTLIST			1
#define	D3D_PT_LINELIST				2
#define	D3D_PT_LINESTRIP			3
#define	D3D_PT_TRIANGLELIST			4
#define	D3D_PT_TRIANGLESTRIP		5
#define	D3D_PT_LINELIST_ADJ			10
#define	D3D_PT_LINESTRIP_ADJ		11
#define	D3D_PT_TRIANGLELIST_ADJ		12
#define	D3D_PT_TRIANGLESTRIP_ADJ	13
// --==</draw_modes_dx11>==--

// --==<draw_modes_nw>==--
#define NW_POINT		3
#define NW_LINE			2
#define NW_FILL			1
// --primitives
#define NW_POINTS			D3D_PT_POINTLIST
#define NW_LINES			D3D_PT_LINELIST
#define NW_LINE_STRIP		D3D_PT_LINESTRIP
#define NW_LINE_LOOP		D3D_PT_LINESTRIP_ADJ
#define NW_TRIANGLES		D3D_PT_TRIANGLELIST
#define NW_TRIANGLE_STRIP	D3D_PT_TRIANGLESTRIP
#define NW_TRIANGLE_FAN		D3D_PT_TRIANGLESTRIP_ADJ
// --==</draw_modes_nw>==--

// --==<planes_dx11>==--
// --==</planes_dx11>==--

// --==<planes_nw>==--
#define NW_FRONT			5
#define NW_BACK				4
#define NW_LEFT				3
#define NW_RIGHT			2
#define NW_FRONT_AND_BACK	1
// --==</planes_nw>==--

// --==<shaders_dx11>==--
// --==<shaders_dx11>==--

// --==<shaders_nw>==--
#define NW_SHADER				4
#define NW_SHADER_VERTEX		3
#define NW_SHADER_GEOMETRY		2
#define NW_SHADER_PIXEL			1
// --==</shaders_nw>==--

// --==<configurations_dx11>==--
// --==</configurations_dx11>==--

// --==<configurations_nw>==--
#define NW_POINT_SIZE			8
#define NW_LINE_SMOOTH			7
#define NW_LINE_WIDTH			6

#define NW_CULL_FACE			5
#define NW_CULL_FACE_MODE		4
#define NW_FRONT_FACE			3
#define NW_CW					2
#define NW_CCW					1

#define NW_DEPTH_TEST			51
#define NW_DEPTH_FUNC			52
#define NW_STENCIL_TEST			53
#define NW_STENCIL_FUNC			54

#define NW_BLEND				1
#define NW_BLEND_DST			2
#define NW_BLEND_SRC			3
#define NW_SRC_COLOR			4
#define NW_ONE_MINUS_SRC_COLOR	5
#define NW_SRC_ALPHA			6
#define NW_ONE_MINUS_SRC_ALPHA	7
#define NW_DST_ALPHA			8
#define NW_ONE_MINUS_DST_ALPHA	9
#define NW_DST_COLOR			10
#define NW_ONE_MINUS_DST_COLOR	11
#define NW_SRC_ALPHA_SATURATE	12

#define NW_MULTISAMPLE			0

#define NW_KEEP					0
#define NW_REPLACE				0
#define NW_INCR					0
#define NW_DECR					0
// --==</configurations_nw>==--

// --==<expressions_dx11>==--
// --constants
// --conditions
// --logic
// --==</expressions_dx11>==--

// --==<expressions_nw>==--
// --constants
#define NW_NONE		0
#define NW_FALSE	0
#define NW_TRUE		1
// --conditions
#define NW_NEVER		0x0200
#define NW_LESS			0x0201
#define NW_EQUAL		0x0202
#define NW_LEQUAL		0x0203
#define NW_GREATER		0x0204
#define NW_NOTEQUAL		0x0205
#define NW_GEQUAL		0x0206
#define NW_ALWAYS		0x0207
// --logic
#define NW_AND				0x1501
#define NW_AND_REVERSE		0x1502
#define NW_COPY				0x1503
#define NW_AND_INVERTED		0x1504
#define NW_NOOP				0x1505
#define NW_XOR				0x1506
#define NW_OR				0x1507
#define NW_NOR				0x1508
#define NW_EQUIV			0x1509
#define NW_INVERT			0x150A
#define NW_OR_REVERSE		0x150B
#define NW_COPY_INVERTED	0x150C
#define NW_OR_INVERTED		0x150D
#define NW_NAND				0x150E
// --==</expressions_nw>==--

// --==<pixel_formats_dx11>==--
// --==</pixel_formats_dx11>==--

// --==<pixel_formats_nw>==--
#define NW_RED				1
#define NW_GREEN			2
#define NW_BLUE				3
#define NW_ALPHA			4
#define NW_RGB				5
#define NW_RGBA				6
#define NW_RED_INT			7
#define NW_GREEN_INT		8
#define NW_BLUE_INT			9
#define NW_RGB_INT			10
#define NW_RGBA_INT			11
#define NW_DEPTH			12
#define NW_STENCIL			13
#define NW_RED_INT32		14
#define NW_RED_UINT32		15
#define NW_RGBA8			16
#define NW_DEPTH24			17
#define NW_STENCIL8			18
#define NW_DEPTH24_STENCIL8	19
// --==</pixel_formats_nw>==--

// --==<textures_dx11>==--
// --==</textures_dx11>==--

// --==<textures_nw>==--
// --types
#define NW_TEXTURE_1D					1
#define NW_TEXTURE_2D					2
#define NW_TEXTURE_3D					3
#define NW_TEXTURE_2D_MULTISAMPLE		4
#define NW_TEXTURE_3D_MULTISAMPLE		5
#define NW_DEPTH_STENCIL				6
#define NW_DEPTH_STENCIL_ATTACHMENT		7
// --filters
#define NW_TEXTURE_MAG_FILTER			1
#define NW_TEXTURE_MIN_FILTER			2
#define NW_TEXTURE_FILTER_NEAREST		3
#define NW_TEXTURE_FILTER_LINEAR		4
// --wrappers
#define NW_TEXTURE_WRAP_S				5
#define NW_TEXTURE_WRAP_T				6
#define NW_TEXTURE_WRAP_R				7
#define NW_TEXTURE_WRAP_CLAMP			8
#define NW_TEXTURE_WRAP_REPEAT			9
// --==</textures_nw>==--

// --==<gbuffers_dx11>==--
// --==</gbuffers_dx11>==--


// --==<gbuffers_nw>==--
#define NWG_BUFFER_VERTEX	1
#define NWG_BUFFER_INDEX	2
#define NWG_BUFFER_SHADER	3
// --==</gbuffers_nw>==--

// --==<framebuffers_dx11>==--
// --==</framebuffers_dx11>==--

// --==<framebuffers_nw>==--
#define NW_FRAMEBUF_IN			1
#define NW_FRAMEBUF_OUT			2
#define NW_FRAMEBUF_IN_OUT		3
#define NW_BUFFER_COLOR_BIT		4
#define NW_BUFFER_DEPTH_BIT		5
#define NW_BUFFER_STENCIL_BIT	6
// --==</framebuffers_nw>==--

// --==<data_load_types_dx11>==--
// --==</data_load_types_dx11>==--

// --==<data_load_types_nw>==--
#define NW_STATIC_DRAW		1
#define NW_STREAM_DRAW		2
#define NW_DYNAMIC_DRAW		3
// --==</data_load_types_nw>==--

// --==<error_codes_dx11>==--
// --==</error_codes_dx11>==--

// --==<error_codes_nw>==--
#define NW_OK					0
#define NW_INVALID_ENUM			1
#define NW_INVALID_VALUE		2
#define NW_INVALID_OPERATION	3
#define NW_STACK_OVERFLOW		4
#define NW_STACK_UNDERFLOW		5
#define NW_OUT_OF_MEMORY		6
// --==</error_codes_nw>==--

// --==<info_dx11>==--
// --==</info_dx11>==--

// --==<info_nw>==--
#define NW_COMPILE_STATUS		7
#define NW_LINK_STATUS			8
// --==</info_nw>==--

// --==<types_dx>==--
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
// --==</types_dx>==--
#endif	// NW_GAPI

namespace NW
{
	class NW_API GfxEngine;
	class NW_API GfxEngineOgl;
	class NW_API GfxEngineDx;

	class NW_API Shader;
	class NW_API ShaderOgl;
	class NW_API ShaderDx;
	class NW_API Texture;
	class NW_API TextureOgl;
	class NW_API TextureDx;
	class NW_API GfxMaterial;

	class NW_API FrameBuf;

	class NW_API VertexBufLayout;
	class NW_API ShaderBufLayout;

	class NW_API VertexBuf;
	class NW_API VertexBufOgl;
	class NW_API VertexBufDx;
	
	class NW_API IndexBuf;
	class NW_API IndexBufOgl;
	class NW_API IndexBufDx;
	
	class NW_API ShaderBuf;
	class NW_API ShaderBufOgl;
	class NW_API ShaderBufDx;

	class NW_API VertexArr;
	class NW_API VertexArrOgl;
	class NW_API VertexArrDx;
}
namespace NW
{
	struct NW_API Drawable;

	struct NW_API GfxCamera;

	struct NW_API FrameBufInfo;
	struct NW_API TextureInfo;

	struct NW_API BufferElement;

	struct NW_API SubTexture1d;
	struct NW_API SubTexture2d;
	struct NW_API SubTexture3d;
}
namespace NW
{
	enum GfxApiTypes : UInt32 {
		GAPI_DEFAULT = NW_GAPI,
		GAPI_OPENGL = NW_GAPI_OGL,
		GAPI_DIRECTX = NW_GAPI_DX,
	};
	enum GfxBufferTypes : UInt32 {
		GBT_DEFAULT = NWG_BUFFER_VERTEX,
		GBT_VERTEX = NWG_BUFFER_VERTEX,
		GBT_INDEX = NWG_BUFFER_INDEX,
		GBT_SHADER = NWG_BUFFER_SHADER,
	};
	enum FrameBufTypes : UInt32 {
		FBT_DEFAULT = NW_FRAMEBUF_IN_OUT,
		FBT_IN = NW_FRAMEBUF_IN, FBT_OUT = NW_FRAMEBUF_OUT, FBT_IN_OUT = NW_FRAMEBUF_IN_OUT,
	};
	enum ShaderTypes : UInt32 {
		ST_DEFAULT = NW_SHADER,
		ST_SHADER = NW_SHADER,
		ST_VERTEX = NW_SHADER_VERTEX,
		ST_GEOMETRY = NW_SHADER_GEOMETRY,
		ST_PIXEL = NW_SHADER_PIXEL
	};
	enum FrameBufs : UInt32 {
		FB_DEFAULT = NW_BUFFER_COLOR_BIT | NW_BUFFER_DEPTH_BIT | NW_BUFFER_STENCIL_BIT,
		FB_COLOR = NW_BUFFER_COLOR_BIT,
		FB_DEPTH = NW_BUFFER_DEPTH_BIT,
		FB_STENCIL = NW_BUFFER_STENCIL_BIT
	};
	enum TextureTypes : UInt32 {
		TXT_NONE = 0,
		TXT_1D = NW_TEXTURE_1D, TXT_2D = NW_TEXTURE_2D, TXT_3D = NW_TEXTURE_3D,
		TXT_2D_MULTISAMPLE = NW_TEXTURE_2D_MULTISAMPLE, TXT_3D_MULTISAMPLE = NW_TEXTURE_3D_MULTISAMPLE
	};
	enum ShaderDataTypes : UInt32 {
		SDT_DEFAULT = NW_FLOAT32,
		SDT_BOOL = NW_BOOL, SDT_INT8 = NW_INT8, SDT_UINT8 = NW_UINT8,
		SDT_INT16 = NW_INT16, SDT_UINT16 = NW_UINT16,
		SDT_INT32 = NW_INT32, SDT_UINT32 = NW_UINT32,
		SDT_FLOAT32 = NW_FLOAT32, SDT_FLOAT64 = NW_FLOAT64,

		SDT_SAMPLER = NW_SAMPLER_1D
	};
}
namespace NW
{
	enum GfxPrimitives : UInt32 {
		GPT_DEFAULT = NW_TRIANGLES,
		GPT_POINTS = NW_POINTS,
		GPT_LINES = NW_LINES,
		GPT_LINE_LOOP = NW_LINE_LOOP,
		GPT_LINE_STRIP = NW_LINE_STRIP,
		GPT_TRIANGLES = NW_TRIANGLES,
		GPT_TRIANGLE_STRIP = NW_TRIANGLE_STRIP,
		GPT_TRIANGLE_FAN = NW_TRIANGLE_FAN,
	};
	enum DrawModes : UInt32 {
		DM_DEFAULT = NW_FILL,
		DM_LINE = NW_LINE, DM_FILL = NW_FILL
	};
	enum FacePlanes : UInt32 {
		FACE_DEFAULT = NW_FRONT_AND_BACK,
		FACE_FRONT_AND_BACK = NW_FRONT_AND_BACK,
		FACE_BACK = NW_BACK, FACE_FRONT = NW_FRONT,
		FACE_LEFT = NW_LEFT, FACE_RIGTH = NW_RIGHT,
	};
	enum GfxVariables : UInt32 {
		GV_DEFAULT = NW_LINE_WIDTH,
		GV_LINE_WIDTH = NW_LINE_WIDTH, GV_POINT_SIZE = NW_POINT_SIZE,
	};
	enum ProcessingModes : UInt32 {
		PM_BLEND = NW_BLEND,
		PM_MULTISAMPLE = NW_MULTISAMPLE,
		PM_CULL_FACE = NW_CULL_FACE,
		PM_DEPTH_TEST = NW_DEPTH_TEST,
		PM_STENCIL_TEST = NW_STENCIL_TEST
	};
	enum BlendConfigs : UInt32 {
		BC_SRC_DEFAULT = 0, BC_DEST_DEFAULT = 0,
		BC_SRC_ALPHA = NW_SRC_ALPHA, BC_SRC_COLOR = NW_SRC_COLOR,
		BC_ONE_MINUS_SRC_ALPHA = NW_ONE_MINUS_SRC_ALPHA,
		BC_ONE_MINUS_SRC_COLOR = NW_ONE_MINUS_SRC_COLOR,
		BC_DST_ALPHA = NW_DST_ALPHA, BC_DST_COLOR = NW_DST_COLOR,
		BC_ONE_MUNUS_DST_ALPHA = NW_ONE_MINUS_DST_ALPHA,
		BC_ONE_MINUS_DST_COLOR = NW_ONE_MINUS_DST_COLOR,
	};
	enum CullFaceConfigs : UInt32 {
		CFC_DEFAULT = 0
	};
	enum DepthConfigs : UInt32 {
		DTC_DEFAULT = NW_GREATER,
		DTC_EQUAL = NW_EQUAL,
		DTC_LESS = NW_LESS, DTC_LEQUAL = NW_LEQUAL,
		DTC_GREATER = NW_GREATER, DTC_GEQUAL = NW_GEQUAL,
		DTC_NEVER = NW_NEVER, DTC_ALWAYS = NW_ALWAYS
	};
	enum StencilConfigs : UInt32 {
		STC_DEFAULT = 0,
		STC_KEEP = NW_KEEP, STC_ZERO = NW_ZERO, STC_REPLACE = NW_REPLACE, STC_INVERT = NW_INVERT,
		STC_INCR = NW_INCR, STC_DECR = NW_DECR
	};
}
namespace NW
{
	enum TextureWraps : UInt32 {
		TXW_NONE = 0, TXW_REPEAT = NW_TEXTURE_WRAP_REPEAT, TXW_CLAMP = NW_TEXTURE_WRAP_CLAMP,
	};
	enum TextureFilters : UInt32 {
		TXF_LINEAR = NW_TEXTURE_FILTER_LINEAR, TXF_NEAREST = NW_TEXTURE_FILTER_NEAREST,
	};
	enum TextureFormats : UInt32 {
		TXF_NONE = 0,
		TXF_RED = NW_RED, TXF_GREEN = NW_GREEN, TXF_BLUE = NW_BLUE,
		TXF_RGB = NW_RGB, TXF_RGBA = NW_RGBA,
		TXF_RED_INT = NW_RED_INT, TXF_GREEN_INT = NW_GREEN_INT, TXF_BLUE_INT = NW_BLUE_INT,
		TXF_RGB_INT = NW_RGB_INT, TXF_RGBA_INT = NW_RGBA_INT,
		TXF_DEPTH = NW_DEPTH, TXF_STENCIL = NW_STENCIL,
		TXF_DEPTH_STENCIL = NW_DEPTH_STENCIL,
	};
	enum TextureInterFormats : UInt32 {
		TXFI_RED_UINT32 = NW_RED_UINT32, TXFI_RED_INT32 = NW_RED_INT32,
		TXFI_RGB = NW_RGB, TXFI_RGBA8 = NW_RGBA8,
		TXFI_DEPTH24 = NW_DEPTH24, TXFI_STENCIL8 = NW_STENCIL8,
		TXFI_DEPTH24_STENCIL8 = NW_DEPTH24_STENCIL8,
	};
	enum PixelFormats : UInt32 {
		FBAT_DEFAULT = 0,
		PXF_INT8 = NW_INT8, PXF_UINT8 = NW_UINT8,
		PXF_INT32 = NW_INT32, PXF_UINT32 = NW_UINT32,
		PXF_UINT24_8 = NW_UINT24_8
	};
	//	--==</enums>==--
}

#endif	// NWG_CORE_HPP

/*
* The project created in 16.01.2021
* Graphics library as a tool for game engines and applications
*/