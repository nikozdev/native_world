#ifndef NW_GMATERIAL_H
#define NW_GMATERIAL_H

#include <gl/vision/nw_shader.h>
#include <gl/render/nw_texture.h>

namespace NW
{
	/// Abstract GraphicsMaterial Class
	/// Description:
	/// -- GMaterial has at least one texture and one shader
	/// -- To use materials - make derived class and use abstract pointer to it's instances
	class NW_API AGMaterial : public ADataRes
	{
	public:
		AGMaterial(const char* strName);
		virtual ~AGMaterial();

		// -- Getters
		virtual inline AShader* GetShader() = 0;
		virtual inline ATexture* GetTexture(CString strType = "") = 0;
		virtual inline V4f GetColor(CString strType = "") = 0;
		// -- Setters
		virtual void SetShader(AShader* pShader) = 0;
		virtual void SetTexture(ATexture* pTex = nullptr, CString strType = "") = 0;
		virtual void SetColor(V4f rgbaClr = V4f(1.0f), CString strType = "") = 0;
		
		// -- Core Methods
		virtual void Enable() = 0;
		virtual void Disable() = 0;
		// -- Data Methods
		virtual bool SaveF(const char* strFPath) = 0;
		virtual bool LoadF(const char* strFPath) = 0;
	public:
		HashMap<const char*, ATexture*> m_Textures;
	};
	/// Default GraphicsMaterial 2d
	class NW_API GMaterial2d : public AGMaterial
	{
	public:
		GMaterial2d(const char* strName);
		~GMaterial2d();

		// -- Getters
		virtual inline AShader* GetShader() override { return m_pShader; }
		virtual inline ATexture* GetTexture(CString strName = "") override { return m_pTex; }
		virtual inline V4f GetColor(CString strType = "") override { return m_rgbaClr; }
		// -- Setters
		virtual void SetShader(AShader* pShader) override { m_pShader = pShader; }
		virtual void SetTexture(ATexture* pTex = nullptr, CString strType = "") override { m_pTex = pTex; }
		virtual void SetColor(V4f rgbaClr, CString strType = "") override { m_rgbaClr = rgbaClr; }

		// -- Core Methods
		virtual void Enable() override;
		virtual void Disable() override;

		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
		// -- Data Methods
	private:
		AShader* m_pShader;
		ATexture* m_pTex;
		V4f m_rgbaClr;
	};

	class NW_API GMaterial3d : public AGMaterial
	{
	public:
		GMaterial3d(const char* strName);
		~GMaterial3d();

		// -- Getters
		virtual inline AShader* GetShader() override { return m_pShader; }
		virtual inline ATexture* GetTexture(CString strName = "") override { return m_pTex; }
		virtual inline V4f GetColor(CString strType = "") override { return m_rgbaClr; }
		// -- Setters
		virtual void SetShader(AShader* pShader) override { m_pShader = pShader; }
		virtual void SetTexture(ATexture* pTex = nullptr, CString strType = "") override { m_pTex = pTex; }
		virtual void SetColor(V4f rgbaClr, CString strType = "") override { m_rgbaClr = rgbaClr; }

		// -- Core Methods
		virtual void Enable() override;
		virtual void Disable() override;

		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
		// -- Data Methods
	private:
		AShader* m_pShader;
		ATexture* m_pTex;
		V4f m_rgbaClr;
	};
}

#endif // NW_AGMATERIAL_H