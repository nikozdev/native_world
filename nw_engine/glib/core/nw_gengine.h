#ifndef NW_GENGINE_H
#define NW_GENGINE_H

#include <glib/core/nw_glayer.h>

namespace NW
{
	/// GEngine singleton class
	class NW_API GEngine : public ASingleton<GEngine>
	{
	public:
		using Layers = DArray<GLayer>;
		friend class ASingleton<GEngine>;
	private:
		GEngine();
		GEngine(const GEngine& rCpy) = delete;
		void operator=(const GEngine& rCpy) = delete;
	public:
		~GEngine();
		
		// --getters
		inline Thread& GetRunThread() { return m_thrRun; }
		inline AMemAllocator& GetMemory() { return m_Memory; }
		
		inline AGApi* GetGApi() { return m_pGApi.GetRef(); }
		const GEngineInfo& GetInfo() { return m_DInfo; }
		inline Layers& GetLayers() { return m_GLayers; }
		inline GLayer* GetLayer() { return &*m_GLayer; }
		inline GLayer* GetLayer(const char* strName);
		// --setters
		GLayer* AddLayer(const char* strName);
		void RmvLayer(const char* strName);
		// --predicates
		Bit IsRunning() { return m_bIsRunning; }

		// --core_methods
		bool Init(Size szMem);
		void Quit();
		void Run();
		void Update();
	private:
		Thread m_thrRun;
		MemArena m_Memory;
		Bit m_bIsRunning;

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