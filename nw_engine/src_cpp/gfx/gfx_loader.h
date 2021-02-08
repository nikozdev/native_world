#if (defined GFX_GAPI)
	#if (GFX_GAPI & GFX_GAPI_OGL)
			#include <../ext/glad.h>
		#if (defined GFX_PLATFORM_WINDOWS)
			#include <../ext/glad_wgl.h>
		#endif
	#endif
		#include <nwl_windows.h>
		//#include <windows.graphics.h>
	#if (GFX_GAPI & GFX_GAPI_DX)
		#include <d3d11.h>
		//#include <windows.graphics.directx.h>
		//#include <windows.graphics.directx.direct3d11.h>
	#endif
#endif