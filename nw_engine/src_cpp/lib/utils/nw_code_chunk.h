#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include <core/nw_data_res.h>

#include <lua_decl.hpp>

namespace NW
{
	/// Abstract CodeChunk class
	/// Description:
	/// -- For classes which can handle some source code and compile that
	/// -- Scripts and shaders need to be edited, loaded, saved and compiled.
	class NW_API ACodeChunk : public ADataRes
	{
	public:
		ACodeChunk(const char* strName);
		virtual ~ACodeChunk();

		// -- Getters
		inline const char* GetCode() { return &m_strCode[0]; }
		// -- Setters
		inline void SetCode(const char* strCode) { m_strCode = strCode; }
		// -- Interface Methods
		virtual bool Compile() = 0;
		virtual bool SaveF(const char* strFPath) = 0;
		virtual bool LoadF(const char* strFPath) = 0;
	protected:
		String m_strCode;
	};
	/// LuaScript class
	class NW_API LuaScript : public ACodeChunk
	{
	public:
		LuaScript(const char* strName);
		~LuaScript();

		// -- Getters
		// -- Setters

		// -- Interface Methods
		virtual bool Compile() override;
		// -- Data Methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
	};
}

#endif	// LUA_SCRIPT_H