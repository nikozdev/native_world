#ifndef NW_GRAPHICS_ENGINE_H
#define NW_GRAPHICS_ENGINE_H

#include <glib/control/nw_graph_state.h>
#include <glib/control/nw_graph_api.h>
#include <glib/nw_glib_core.h>

#include <lib/nw_singleton.h>

namespace NW
{
	/// GraphEngine Singleton Class
	/// Description:
	/// -- This is the secondary singleton Draw class (GraphEngine can not live without GraphEngine3d)
	/// -- GraphEngine3d handles Graphics API and provides an interface for drawing any AShape3d we have
	/// -- GraphEngine has index and vertex buffers, arrays, materials and shaders for drawing AShape2d
	/// -- All Draw must to be done betweeen calls BeginDraw() and EndDraw() in some place inside the
	/// application while-loop
	/// -- It uses batch Drawing technique to draw everything we ask for as less as possible "draw calls"
	/// -- Because of batch Drawing we have a great performance but
	/// maximum amount of 2d shapes is restriced as well as textures we can set in just 1 time.
	/// Interface:
	/// -> Initialize graphics context -> Set GraphEngine's GAPI type
	/// -> Call the Init method -> BeginDraw with some scene which has a camera
	/// -> OnDraw all the objects you need -> EndDraw
	class NW_API GraphEngine : public ASingleton<GraphEngine>
	{
	public:
		using States = HashMap<String, GraphState*>;
		friend class ASingleton<GraphEngine>;
	private:
		GraphEngine();
		GraphEngine(const GraphEngine& rCpy) = delete;
		void operator=(const GraphEngine& rCpy) = delete;
	public:
		~GraphEngine();
		
		// --getters
		inline AGraphApi* GetGApi() { return m_pGApi; }
		const GraphEngineInfo& GetInfo() { return m_DInfo; }
		inline States& GetStates() { return m_States; }
		inline GraphState* GetState() { return m_State->second; }
		inline GraphState* GetState(const char* strName) {
			States::iterator itState = m_States.find(strName);
			return itState == m_States.end() ? nullptr : itState->second;
		}
		void AddState(GraphState* pState) {
			if (pState == nullptr) { return; }
			if (GetState(pState->GetName()) != nullptr) { return; }
			m_States[pState->GetName()] = pState;
			if (m_State == m_States.end()) { m_State = m_States.find(pState->GetName()); }
		}
		void RmvState(const char* strName) {
			States::iterator itDest = m_States.find(strName);
			if (itDest != m_States.end()) { return ; }
			if (m_State == itDest) { m_State = m_States.begin(); }
			m_States.erase(itDest);
		}
		// --setters
		void SetMaxVtxSize(Size szMaxVtx);
		void SetMaxIdxSize(Size szMaxIdx);
		void SetMaxShdSize(Size szMaxShd);
		void SetMaxTexCount(UInt8 unMaxTex);
		// -- Predicates
		bool IsRunning() { return m_bIsRunning; }

		// -- Core methods
		bool Init(GApiTypes GraphicsApiType);
		void OnQuit();
		void Update();
	private: // Implementation Methods
		inline void UploadDrawData(ADrawable* pDrawable);
		inline void UploadSceneData(DrawSceneData* pDSData);
		inline void DrawCall();
	private: // Implementation Attributes
		AGraphApi* m_pGApi;
		States m_States;
		States::iterator m_State;

		GraphEngineInfo m_DInfo;
		bool m_bIsRunning;
	};
}

#endif // NW_DRAWER_H