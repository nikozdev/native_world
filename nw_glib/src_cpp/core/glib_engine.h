#ifndef GLIB_ENGINE_H
#define GLIB_ENGINE_H

#include <core/glib_layer.h>

namespace GLIB
{
	/// GEngine singleton class
	class GLIB_API GEngine : public ASingleton<GEngine>
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
		inline AMemAllocator& GetMemory() { return m_Memory; }
		inline Thread& GetRunThread() { return m_thrRun; }
		
		inline AGApi* GetGApi() { return m_pGApi; }
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
		bool Init(Size szMemory);
		void Quit();
		void Run();
		void Update();
		// --data_methods
		bool SaveFImage(const char* strFPath, ImageInfo* pImgInfo);
		bool LoadFImage(const char* strFPath, ImageInfo* pImgInfo);
		bool SaveFShaderCode(const char* strFPath, AShader* pShader);
		bool LoadFShaderCode(const char* strFPath, AShader* pShader);
	private:
		Bit m_bIsRunning;
		Thread m_thrRun;
		MemArena m_Memory;

		AGApi* m_pGApi;
		GMaterial* m_pGMtlScreen;

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

#endif // GLIB_ENGINE_H