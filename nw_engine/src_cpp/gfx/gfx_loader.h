#ifndef NWG_LOADER_H
#define NWG_LOADER_H

#if (defined NW_GAPI)
	#if (NW_GAPI & NW_GAPI_OGL)
		//#include <gl/gl.h>
		#include <../ext/glad/glad.h>
		#pragma comment(lib, "opengl32.lib")
	#endif
	#if (NW_GAPI & NW_GAPI_DX)
		#include <d3d11.h>
		#include <d3dcompiler.h>
		#pragma comment(lib, "d3d11.lib")
		#pragma comment(lib, "d3dcompiler.lib")
	#endif
#endif

#endif	// NWG_LOADER_H