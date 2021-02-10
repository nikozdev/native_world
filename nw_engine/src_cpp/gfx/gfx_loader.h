#ifndef NW_GFX_LOADER_H
#define NW_GFX_LOADER_H

#if (defined NW_GAPI)
	#if (NW_GAPI & NW_GAPI_OGL)
		#include <../ext/glad/glad.h>
	#endif
		//#include <windows.graphics.h>
	#if (NW_GAPI & NW_GAPI_DX)
		#include <d3d11.h>
		//#include <windows.graphics.directx.h>
		//#include <windows.graphics.directx.direct3d11.h>
	#endif
#endif

#endif	// NW_GFX_LOADER_H