#shader_type vertex
#version 330 core

layout(location = 0) in vec2 atb_vtxCrd;
layout(location = 1) in vec2 atb_texCrd;

out vec3 vs_vtxCrd;
out vec2 vs_texCrd;

uniform mat4 unf_m4Model;
uniform mat4 unf_m4Proj;
uniform mat4 unf_m4View;

void main()
{
	vs_vtxCrd = vec3(unf_m4Model * vec4(atb_vtxCrd.xy, 0.0f, 1.0f));
	vs_texCrd = atb_texCrd;

	gl_Position = unf_m4Proj * unf_m4View * vec4(vs_vtxCrd.xyz, 1.0f);
}

#shader_type pixel
#version 330 core

#define MAX_TEXTURES 5

out vec4 ps_pxClr;

in vec3 vs_vtxCrd;
in vec2 vs_texCrd;

uniform vec4 unf_albedoClr;
uniform vec3 unf_ambientClr;
uniform vec3 unf_specularClr;
uniform float unf_shining;
uniform sampler2D unf_textures[MAX_TEXTURES];

struct DirectLight
{
	vec3 lClr;
	vec3 lDir;
};
struct PointLight {
	vec3 lClr;
	vec3 lCrd;

	float atn_const;
	float atn_linear;
	float atn_quad;
};
struct SpotLight {
	vec3 lClr;
	vec3 lDir;
	vec3 lCrd;

	float atn_const;
	float atn_linear;
	float atn_quad;

	float cutOff_in;
	float cutOff_out;
};
// -- Structures

// --Ligthing Calculation
float GetDiffuseF(vec3 normCrd, vec3 lDir);
float GetSpecularF(vec3 normCrd, vec3 viewDir, vec3 lDir);

vec4 GetAmbientRgba(vec3 lClr);
vec4 GetDiffuseRgba(vec3 normCrd, vec3 lDir, vec3 lClr);
vec4 GetSpecularRgba(vec3 normCrd, vec3 viewDir, vec3 lDir, vec3 lClr);

vec4 GetDirectLighting(DirectLight dtLight, vec3 normCrd, vec3 viewDir);
vec4 GetPointLighting(PointLight ptLight, vec3 normCrd, vec3 viewDir, vec3 vtxCrd);
vec4 GetSpotLighting(SpotLight stLight, vec3 normCrd, vec3 viewDir, vec3 vtxCrd);
// --Ligthing Calculation

// --Uniforms
uniform vec3 unf_viewCrd;

uniform GMaterial mtl_core_2d;

#define MAX_LIGHTS_DIRECT 10
#define MAX_LIGHTS_POINT 10
#define MAX_LIGHTS_SPOT 10

uniform DirectLight directLight[MAX_LIGHTS_DIRECT];
uniform PointLight pointLight[MAX_LIGHTS_POINT];
uniform SpotLight spotLight[MAX_LIGHTS_SPOT];
// --Uniforms

void main()
{
	vec3 viewDir = normalize(unf_viewCrd - vs_vtxCrd);
	
	vec4 directL = vec4(0.0f);
	vec4 pointL = vec4(0.0f);
	vec4 spotL = vec4(0.0f);
	for (int i = 0; i < MAX_LIGHTS_DIRECT; i++)
	{
		directL += GetDirectLighting(directLight[i], vec3(0.0f, 0.0f, 1.0f), viewDir);
	}
	for (int i = 0; i < MAX_LIGHTS_POINT; i++)
	{
		pointL += GetPointLighting(pointLight[i], vec3(0.0f, 0.0f, 1.0f), viewDir, vs_vtxCrd);
	}
	for (int i = 0; i < MAX_LIGHTS_SPOT; i++)
	{
		spotL += GetSpotLighting(spotLight[i], vec3(0.0f, 0.0f, 1.0f), viewDir, vs_vtxCrd);
	}

	ps_pxClr = directL + pointL + spotL;
	ps_pxClr = vec4(1.0f);
}

// -- LightComponents
float GetDiffuseF(vec3 normCrd, vec3 lDir)
{		// The more vectors of light and normCrd are same - the more light is there
	return (max(dot(normCrd, normalize(-lDir)), 0.0f));
}
float GetSpecularF(vec3 normCrd, vec3 viewDir, vec3 lDir)
{		// The more vectors of light and reflection are same - the more light is there
	vec3 reflectDir = reflect(normCrd, normalize(lDir));
	return (pow(max(dot(reflectDir, viewDir), 0.0f), mtl_core_2d.shining));
}

vec4 GetAmbientRgba(vec3 lClr)
{
	vec4 resClr = vec4(0.0f);
	for (int tx = 0; tx < MAX_TEXTURES; tx++)
		resClr += (texture(mtl_core_2d.textures[tx], vs_texCrd).rgba * vec4(lClr.xyz, 1.0f));
	return resClr;
}
vec4 GetDiffuseRgba(vec3 normCrd, vec3 lDir, vec3 lClr)
{
	vec4 resClr = vec4(0.0f);
	for (int tx = 0; tx < MAX_TEXTURES; tx++)
		resClr += (texture(mtl_core_2d.textures[tx], vs_texCrd).rgba *
			GetDiffuseF(normCrd, lDir) * vec4(lClr.xyz, 1.0f));
	return resClr;
}
vec4 GetSpecularRgba(vec3 normCrd, vec3 viewDir, vec3 lDir, vec3 lClr)
{
	vec4 resClr = vec4(0.0f);
	for (int tx = 0; tx < MAX_TEXTURES; tx++)
		resClr += (texture(mtl_core_2d.textures[tx], vs_texCrd).rgba *
			GetSpecularF(normCrd, lDir, viewDir) * vec4(lClr.xyz, 1.0f));
	return resClr;
}
// -- LightComponents

// -- LightSources
vec4 GetDirectLighting(DirectLight lDt, vec3 normCrd, vec3 viewDir)
{
	vec4 ambient = GetAmbientRgba(lDt.lClr);
	vec4 diffuse = GetDiffuseRgba(normCrd, lDt.lDir, lDt.lClr);
	vec4 specular = GetSpecularRgba(normCrd, viewDir, lDt.lDir, lDt.lClr);
	
	return (ambient + diffuse + specular);
}
vec4 GetPointLighting(PointLight lPt, vec3 normCrd, vec3 viewDir, vec3 pxCrd)
{
	vec3 lfDir = normalize(lPt.lCrd - pxCrd);
	
	vec4 ambient = GetAmbientRgba(lPt.lClr);
	vec4 diffuse = GetDiffuseRgba(normCrd, lfDir, lPt.lClr);
	vec4 specular = GetSpecularRgba(normCrd, viewDir, lfDir, lPt.lClr);
	
	return (ambient + diffuse + specular);
}
vec4 GetSpotLighting(SpotLight lSt, vec3 normCrd, vec3 viewDir, vec3 pxCrd)
{
	vec3 lfDir = normalize(lSt.lCrd - pxCrd);

	vec4 ambient = GetAmbientRgba(lSt.lClr);
	vec4 diffuse = GetDiffuseRgba(normCrd, lfDir, lSt.lClr);
	vec4 specular = GetSpecularRgba(normCrd, viewDir, lfDir, lSt.lClr);

	return (ambient + diffuse + specular);
}
// -- LightSources