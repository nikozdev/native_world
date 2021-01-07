#shader_type vertex
#version 400 core

layout(location = 0) in vec3 atb_vtxCrd;
layout(location = 1) in vec4 atb_vtxClr;
layout(location = 2) in vec2 atb_texCrd;
layout(location = 3) in float atb_texSlot;
layout(location = 4) in mat4 atb_m4Transform;

layout(std140) uniform unf_Matrices {
	mat4 unf_m4Proj;
	mat4 unf_m4View;
};

out VS_OUT {
	vec4 vtxClr;
	vec2 texCrd;
	float texSlot;
} vs_out;

void main()
{
	gl_Position = unf_m4Proj * unf_m4View * atb_m4Transform * vec4(atb_vtxCrd.xyz, 1.0f);
	vs_out.vtxClr = atb_vtxClr;
	vs_out.texCrd = atb_texCrd;
	vs_out.texSlot = atb_texSlot;
}

#shader_type pixel
#version 400 core

out vec4 ps_pxClr;

in VS_OUT {
	vec4 vtxClr;
	vec2 texCrd;
	float texSlot;
} vs_in;

uniform sampler2D unf_textures[7];

void main()
{
	ps_pxClr = vec4(texture(unf_textures[int(vs_in.texSlot)], vs_in.texCrd).rgba * vs_in.vtxClr);
}