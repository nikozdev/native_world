#ifndef SCENE_LIGHT_3D_H
#define SCENE_LIGHT_3D_H

#include <gl_decl.hpp>

#include <lib/utils/math_vector.h>

namespace NW
{
	/// Abstract LightSource class
	class NW_API ALightSource3d
	{
	public:
		ALightSource3d() = default;
		virtual ~ALightSource3d() = default;

		// Getters
		inline virtual const String& GetName() const = 0;
		inline virtual const V3f& GetColor() const = 0;
		// Setters
		inline virtual void SetName(const String& name) = 0;
		inline virtual void SetColor(const V3f& colorRGB) = 0;
	};

	/// DirectLight class for 3d lights
	class NW_API DirectLight3d : public ALightSource3d
	{
	public:
		V3f m_v3Direction;

		DirectLight3d() = default;
		DirectLight3d(const char* name,
			const V3f& rgbColor = V3f(0.0f), const V3f& direction = V3f(0.0f)) :
			m_sName(name), m_rgbColor(rgbColor), m_v3Direction(direction) {}

		// Getters
		inline virtual const String& GetName() const override { return m_sName; }
		inline virtual const V3f& GetColor() const override { return m_rgbColor; }
		// Setters
		inline virtual void SetName(const String& name) override { m_sName = name; }
		inline virtual void SetColor(const V3f& rgbColor) override { m_rgbColor = rgbColor; }
	private:
		String m_sName;
		V3f m_rgbColor;
	};
	/// PointLight class for 3d lights
	class NW_API PointLight3d : public ALightSource3d
	{
	public:
		V3f m_v3Coord;

		float m_nAtn_Const;
		float m_nAtn_Linear;
		float m_nAtn_Quad;

		PointLight3d() = default;
		PointLight3d(const char* name,
			const V3f& lightColorRGB = V3f(0.0f), V3f& attenuation = V3f(0.0f)) :
			m_sName(name), m_rgbColor(lightColorRGB),
			m_nAtn_Const(attenuation[0]), m_nAtn_Linear(attenuation[1]), m_nAtn_Quad(attenuation[2]) {}

		// Getters
		inline virtual const String& GetName() const override { return m_sName; }
		inline virtual const V3f& GetColor() const override { return m_rgbColor; }
		// Setters
		inline virtual void SetName(const String& name) override { m_sName = name; }
		inline virtual void SetColor(const V3f& colorRGB) override { m_rgbColor = colorRGB; }
	private:
		String m_sName;
		V3f m_rgbColor;
	};
	/// SpotLight class for 3d lights
	class NW_API SpotLight3d : public ALightSource3d
	{
	public:
		V3f m_v3Direction;

		V3f m_v3Coord;

		float m_nAtn_Const;
		float m_nAtn_Linear;
		float m_nAtn_Quad;

		float m_nAngleIn;
		float m_nAngleOut;

		SpotLight3d() = default;
		SpotLight3d(const char* name,
			const V3f& lightColorRGB = V3f(0.0f), const V3f& direction = V3f(0.0f),
			const V3f& coordinates = V3f(0.0f),
			V3f& attenuation = V3f(0.0f), V2f& cutOffAngle = V2f(0.0f)) :
			m_sName(name), m_rgbColor(lightColorRGB), m_v3Direction(direction),
			m_nAtn_Const(attenuation[0]), m_nAtn_Linear(attenuation[1]), m_nAtn_Quad(attenuation[2]),
			m_nAngleIn(cutOffAngle[0]), m_nAngleOut(cutOffAngle[1]) {}

		// Getters
		inline virtual const String& GetName() const override { return m_sName; }
		inline virtual const V3f& GetColor() const override { return m_rgbColor; }
		// Setters
		inline virtual void SetName(const String& name) override { m_sName = name; }
		inline virtual void SetColor(const V3f& colorRGB) override { m_rgbColor = colorRGB; }
	private:
		String m_sName;

		V3f m_rgbColor;
	};
}

#endif // NW_LIGHT_SOURCE_3D_H