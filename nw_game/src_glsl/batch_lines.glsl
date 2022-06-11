#shader_type vertex
#version 400 core

layout(location = 0) in vec3 atb_vtxCrd;
layout(location = 1) in vec4 atb_vtxClr;
layout(location = 2) in float atb_texCrd;
layout(location = 3) in float atb_texSlot;

out VS_OUT {
	vec3 vtxCrd;
	vec4 vtxClr;
	float texCrd;
	float texSlot;
} vs_out;

uniform mat4 unf_m4View;
uniform mat4 unf_m4Proj;

void main()
{
	vs_out.vtxCrd = atb_vtxCrd;
	vs_out.vtxClr = atb_vtxClr;
	vs_out.texCrd = atb_texCrd;
	vs_out.texSlot = atb_texSlot;

	gl_Position = unf_m4Proj * unf_m4View *vec4(atb_vtxCrd, 1.0f);
}

#shader_type geometry
#version 400 core


in VS_OUT {
	vec3 vtxCrd;
	vec4 vtxClr;
	float texCrd;
	float texSlot;
} vs_in[];

out VS_OUT {
	vec3 vtxCrd;
	vec4 vtxClr;
	float texCrd;
	float texSlot;
} gs_out;

layout(lines) in;
layout(line_strip, max_vertices = 2) out;

void main()
{
	gs_out.vtxCrd = vs_in[0].vtxCrd;
	gs_out.vtxClr = vs_in[0].vtxClr;
	gs_out.texCrd = vs_in[0].texCrd;
	gs_out.texSlot = vs_in[0].texSlot;
	gl_Position = gl_in[0].gl_Position;
	gl_PointSize = gl_in[0].gl_PointSize;
	EmitVertex();
	
	gs_out.vtxCrd = vs_in[1].vtxCrd;
	gs_out.vtxClr = vs_in[1].vtxClr;
	gs_out.texCrd = vs_in[1].texCrd;
	gs_out.texSlot = vs_in[1].texSlot;
	gl_Position = gl_in[1].gl_Position;
	gl_PointSize = gl_in[1].gl_PointSize;
	EmitVertex();

	EndPrimitive();
}

#shader_type pixel
#version 430 core

out vec4 ps_pxClr;

in VS_OUT {
	vec3 vtxCrd;
	vec4 vtxClr;
	float texCrd;
	float texSlot;
} gs_in;

uniform vec4 unf_albedoClr;
uniform sampler1D unf_textures[7];

void main()
{
	ps_pxClr = texture(unf_textures[int(gs_in.texSlot)], gs_in.texCrd) *
		gs_in.vtxClr * unf_albedoClr;
}