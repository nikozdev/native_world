#ifndef NW_GENGINE_H
#define NW_GENGINE_H

#include <glib/core/nw_glayer.h>
#include <glib/core/nw_gapi.h>
#include <glib/nw_glib_core.h>

#include <nwlib/nw_singleton.h>

namespace NW
{
	/// GEngine singleton class
	class NW_API GEngine : public ASingleton<GEngine>
	{
	public:
		using Layers = List2<GLayer>;
		friend class ASingleton<GEngine>;
	private:
		GEngine();
		GEngine(const GEngine& rCpy) = delete;
		void operator=(const GEngine& rCpy) = delete;
	public:
		~GEngine();
		
		// --getters
		inline AWindow* GetWindow() { return m_pWindow; }
		inline AGApi* GetGApi() { return m_pGApi; }
		const GEngineInfo& GetInfo() { return m_DInfo; }
		inline Layers& GetLayers() { return m_GLayers; }
		inline GLayer* GetLayer() { return &*m_GLayer; }
		inline GLayer* GetLayer(const char* strName);
		// --setters
		GLayer* AddLayer(const char* strName);
		void RmvLayer(const char* strName);
		void ChangeLayerOrder(const char* strName, bool bPushUp);
		// -- Predicates
		bool IsRunning() { return m_bIsRunning; }

		// -- Core methods
		bool Init(WApiTypes WindowApiType, GApiTypes GraphicsApiType);
		void Quit();
		void Update();
		void DrawCall(DrawTools& rDTools);
	private:
		bool m_bIsRunning;
		AWindow* m_pWindow;
		AGApi* m_pGApi;

		Layers m_GLayers;
		Layers::iterator m_GLayer;

		GEngineInfo m_DInfo;
	};
	inline GLayer* GEngine::GetLayer(const char* strName) {
		Layers::iterator itLayer = std::find_if(m_GLayers.begin(), m_GLayers.end(),
			[=](GLayer& rObj)->bool {return strcmp(&rObj.GetName()[0], strName) == 0; });
		return itLayer == m_GLayers.end() ? nullptr : &*itLayer;
	}
}

#endif // NW_GENGINE_H