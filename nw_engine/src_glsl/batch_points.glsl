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

	gl_Position = unf_m4Proj * unf_m4View * vec4(atb_vtxCrd, 1.0f);
}

#shader_type geometry
#version 400 core

layout(points) in;
layout(points, max_vertices = 1) out;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	gl_PointSize = gl_in[0].gl_PointSize;
	EmitVertex();

	EndPrimitive();
}

#shader_type pixel
#version 400 core

out vec4 ps_pxClr;

in VS_OUT {
	vec3 vtxCrd;
	vec4 vtxClr;
	float texCrd;
	float texSlot;
} vs_in;

uniform vec4 unf_albedoClr;
uniform sampler1D unf_textures[7];

void main()
{
	ps_pxClr = texture(unf_textures[int(vs_in.texSlot)], vs_in.texCrd).rgba * vs_in.vtxClr * unf_albedoClr;
}