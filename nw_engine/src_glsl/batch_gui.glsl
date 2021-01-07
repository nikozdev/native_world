#shader_type vertex
#version 400 core

layout(location = 0) in vec2 atb_vtxCrd;
layout(location = 1) in vec4 atb_vtxClr;
layout(location = 2) in vec2 atb_texCrd;
layout(location = 3) in float atb_texSlot;

out VS_OUT {
	vec2 vs_vtxCrd;
	vec4 vs_vtxClr;
	vec2 vs_texCrd;
	float vs_texSlot;
} vs_out;

uniform mat4 unf_m4Proj;
uniform mat4 unf_m4View;

void main()
{
	gl_Position = unf_m4Proj * unf_m4View * vec4(atb_vtxCrd.xy, 0.0f, 1.0f);

	vs_out.vtxCrd = vec2(glPosition.xy);
	vs_out.vtxClr = atb_vtxClr;
	vs_out.texCrd = atb_texCrd;
	vs_out.texSlot = atb_texSlot;
}

#shader_type pixel
#version 400 core

out vec4 ps_pxClr;

in VS_OUT {
	vec2 vs_vtxCrd;
	vec4 vs_vtxClr;
	vec2 vs_texCrd;
	float vs_texSlot;
} vs_in;

uniform vec4 unf_albedoClr;
uniform sampler2D unf_textures[7];

void main()
{
	ps_pxClr = texture(unf_textures[int(vs_texSlot)], vec2(vs_texCrd.x, vs_texCrd.y)).rgba * unf_albedoClr * vs_in.vtxClr;
}