#shader_type vertex
#version 400 core

layout(location = 0) in vec2 atb_vtxCrd;
layout(location = 1) in vec2 atb_texCrd;
layout(location = 2) in float atb_texSlot;

out VS_OUT {
	vec2 texCrd;
	float texSlot;
} vs_out;

void main()
{
	gl_Position = vec4(atb_vtxCrd.x, atb_vtxCrd.y, 0.0f, 1.0f);
	
	vs_out.texCrd = atb_texCrd;
	vs_out.texSlot = atb_texSlot;
}

#shader_type geometry
#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec2 texCrd;
	float texSlot;
} gs_in[];

out VS_OUT {
	vec2 texCrd;
	float texSlot;
} gs_out;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	gs_out.texCrd = gs_in[0].texCrd;
	gs_out.texSlot = gs_in[0].texSlot;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	gs_out.texCrd = gs_in[1].texCrd;
	gs_out.texSlot = gs_in[1].texSlot;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	gs_out.texCrd = gs_in[2].texCrd;
	gs_out.texSlot = gs_in[2].texSlot;
	EmitVertex();

	EndPrimitive();
}

#shader_type pixel
#version 400 core

#define MAX_TEXTURES 7

out vec4 ps_pxClr;

in VS_OUT {
	vec2 texCrd;
	float texSlot;
} ps_in;

uniform sampler2D unf_textures[MAX_TEXTURES];

void main()
{
	ps_pxClr = vec4(texture(unf_textures[int(ps_in.texSlot)], ps_in.texCrd).rgba);
}