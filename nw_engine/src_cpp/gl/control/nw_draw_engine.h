#ifndef NW_DRAWER_H
#define NW_DRAWER_H

#include <gl/control/nw_draw_state.h>
#include <gl/control/nw_gapi.h>
#include <gl/nw_gl_core.h>

namespace NW
{
	/// DrawEngine Singleton Class
	/// Description:
	/// -- This is the secondary singleton Draw class (DrawEngine can not live without DrawEngine3d)
	/// -- DrawEngine3d handles Graphics API and provides an interface for drawing any AShape3d we have
	/// -- DrawEngine has index and vertex buffers, arrays, materials and shaders for drawing AShape2d
	/// -- All Draw must to be done betweeen calls BeginDraw() and EndDraw() in some place inside the
	/// application while-loop
	/// -- It uses batch Drawing technique to draw everything we ask for as less as possible "draw calls"
	/// -- Because of batch Drawing we have a great performance but
	/// maximum amount of 2d shapes is restriced as well as textures we can set in just 1 time.
	/// Interface:
	/// -> Initialize graphics context -> Set DrawEngine's GAPI type
	/// -> Call the Init method -> BeginDraw with some scene which has a camera
	/// -> OnDraw all the objects you need -> EndDraw
	class NW_API DrawEngine
	{
	public:
		using States = HashMap<String, DrawState>;
	public:
		// --getters
		static inline AGraphicsApi* GetGApi() { return s_pGApi; }
		static const DrawEngineInfo& GetInfo() { return s_DInfo; }
		static DrawState& GetState(const char* strName) { strcpy(s_DStates[strName].strName, strName); return s_DStates[strName]; }
		// --setters
		static void SetMaxVtxSize(Size szMaxVtx);
		static void SetMaxIdxSize(Size szMaxIdx);
		static void SetMaxShdSize(Size szMaxShd);
		static void SetMaxTexCount(UInt8 unMaxTex);
		// -- Predicates
		static bool IsDrawing() { return s_bIsDrawing; }

		// -- Core methods
		static bool Init(GApiTypes GraphicsApiType);
		static void OnQuit();
		static void Update();

		static void BeginDraw();
		static void OnDraw(ADrawable* pDrb, const char* strStateName);
		static void EndDraw();
	private: // Implementation Methods
		static inline void UploadVtxData(ADrawable* pDrawable);
		static inline void UploadShdData(DrawSceneData* pDSData);
		static inline void DrawCall();
	private: // Implementation Attributes
		static AGraphicsApi* s_pGApi;
		static States s_DStates;

		static DrawEngineInfo s_DInfo;
		static bool s_bIsDrawing;

	};
}

#endif // NW_DRAWER_H