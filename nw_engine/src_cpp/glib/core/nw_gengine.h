#ifndef NW_GENGINE_H
#define NW_GENGINE_H

#include <glib/core/nw_glayer.h>

namespace NW
{
	/// GEngine singleton class
	class NW_API GEngine
	{
	public:
		using Layers = DArray<GLayer>;
	private:
		GEngine();
		GEngine(const GEngine& rCpy) = delete;
		void operator=(const GEngine& rCpy) = delete;
	public:
		~GEngine();
		
		// --getters
		static inline GEngine& Get() { static GEngine s_GEngine; return s_GEngine; }
		inline std::thread& GetRunThread() { return m_thrRun; }
		inline AWindow* GetWindow() { return m_pWindow.get(); }
		inline AGApi* GetGApi() { return m_pGApi.get(); }
		const GEngineInfo& GetInfo() { return m_DInfo; }
		inline Layers& GetLayers() { return m_GLayers; }
		inline GLayer* GetLayer() { return &*m_GLayer; }
		inline GLayer* GetLayer(const char* strName);
		// --setters
		GLayer* AddLayer(const char* strName);
		void RmvLayer(const char* strName);
		// --predicates
		bool IsRunning() { return m_bIsRunning; }

		// --core_methods
		bool Init(WApiTypes WindowApiType, GApiTypes GraphicsApiType);
		void Quit();
		void Run();
		void Update();
	private:
		bool m_bIsRunning;
		std::thread m_thrRun;

		RefOwner<AWindow> m_pWindow;
		RefOwner<AGApi> m_pGApi;

		Layers m_GLayers;
		Layers::iterator m_GLayer;

		GEngineInfo m_DInfo;
	};
	inline GLayer* GEngine::GetLayer(const char* strName) {
		Layers::iterator itLayer = std::find_if(m_GLayers.begin(), m_GLayers.end(),
			[=](GLayer& rObj)->bool { return rObj.strName == strName; });
		return itLayer == m_GLayers.end() ? nullptr : &*itLayer;
	}
}

#endif // NW_GENGINE_H