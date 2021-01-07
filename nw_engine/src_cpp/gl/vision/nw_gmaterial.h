#ifndef NW_GMATERIAL_H
#define NW_GMATERIAL_H

#include <gl/vision/nw_shader.h>
#include <gl/render/nw_texture.h>

namespace NW
{
	/// GraphicsMaterial Class
	/// Description:
	class NW_API GMaterial : public ADataRes
	{
	public:
		using Textures = HashMap<String, ATexture*>;
	public:
		GMaterial(const char* strName);
		virtual ~GMaterial();

		// --getters
		inline AShader* GetShader() { return m_pShader; }
		inline UInt8 GetTexCount() { return m_Textures.size(); }
		inline const Textures& GetTextures(const char* strType = "") { return m_Textures; }
		inline ATexture* GetTexture(const char* strType = "") {
			auto itTex = m_Textures.find(&strType[0]); 
			return itTex == m_Textures.end() ? nullptr : itTex->second;
		}
		// --setters
		void SetShader(AShader* pShader);
		void SetTexture(ATexture* pTex = nullptr, const char* strType = "");
		// -- Predicates
		Int8 HasTexture(ATexture* pTex) {
			auto itTex = m_Textures.begin();
			for (Int8 txi = 0; txi < GetTexCount(); txi++) {
				if (itTex->second == pTex) { return txi; }
				std::advance(itTex, 1);
			}
			return -1;
		}

		// --core_methods
		void Enable();
		void Disable();
		// --data_methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
	public:
		AShader* m_pShader;
		Textures m_Textures;
	};
}

#endif // NW_AGMATERIAL_H