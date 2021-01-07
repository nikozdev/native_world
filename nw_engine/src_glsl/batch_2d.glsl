#shader_type vertex
#version 400 core

layout(location = 0) in vec2 atb_vtxCrd;
layout(location = 1) in vec4 atb_vtxClr;
layout(location = 2) in vec2 atb_texCrd;
layout(location = 3) in float atb_texSlot;

out VS_OUT {
	vec4 vtxClr;
	vec2 texCrd;
	float texSlot;
} vs_out;

uniform mat4 unf_m4Proj;	// Order
uniform mat4 unf_m4View;	// Is Extrimelly important

void main()
{
	gl_Position = unf_m4Proj * unf_m4View * vec4(atb_vtxCrd.xy, 0.0f, 1.0f);

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

uniform sampler2D textures[7];

void main()
{
	ps_pxClr = vec4(texture(textures[int(vs_in.texSlot)], vs_in.texCrd).rgba *
	vs_in.vtxClr);
}