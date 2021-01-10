#shader_type vertex
#version 400 core

layout(location = 0) in vec3 atb_xyCrd;
layout(location = 1) in vec4 atb_vtxClr;
layout(location = 2) in vec2 atb_texCrd;
layout(location = 3) in float atb_texSlot;
layout(location = 4) in mat4 atb_mdlTransform;

layout(std140) uniform unf_Matrices{
	mat4 unf_camProj;
	mat4 unf_camView;
};

out VS_OUT{
	vec4 vtxClr;
	vec2 texCrd;
	float texSlot;
}vs_out;

void main()
{
	gl_Position = unf_camProj * unf_camView * atb_mdlTransform *
	vec4(atb_xyCrd.xy, 0.0f, 1.0f);
	vs_out.vtxClr = atb_vtxClr;
	vs_out.texCrd = atb_texCrd;
	vs_out.texSlot = atb_texSlot;
}

#shader_type geometry
#version 400 core

layout(triangles)in;
layout(triangle_strip,max_vertices=4)out;

in VS_OUT{
	vec4 vtxClr;
	vec2 texCrd;
	float texSlot;
}gs_in[];

out VS_OUT{
	vec4 vtxClr;
	vec2 texCrd;
	float texSlot;
}gs_out;

void main()
{
	gl_Position = vec4(gl_in[0].gl_Position.xyzw);
	gs_out.vtxClr = gs_in[0].vtxClr;
	gs_out.texCrd = gs_in[0].texCrd;
	gs_out.texSlot = gs_in[0].texSlot;
	EmitVertex();
	gl_Position = vec4(gl_in[1].gl_Position.xyzw);
	gs_out.vtxClr = gs_in[0].vtxClr;
	gs_out.texCrd = gs_in[1].texCrd;
	gs_out.texSlot = gs_in[0].texSlot;
	EmitVertex();
	gl_Position = vec4(gl_in[2].gl_Position.xyzw);
	gs_out.vtxClr = gs_in[0].vtxClr;
	gs_out.texCrd = gs_in[2].texCrd;
	gs_out.texSlot = gs_in[0].texSlot;
	EmitVertex();
	gl_Position = vec4(gl_in[0].gl_Position.xyzw) + vec4(gl_in[2].gl_Position.x, 0.0f, 0.0f, 0.0f);
	gs_out.vtxClr = gs_in[0].vtxClr;
	gs_out.texCrd = gs_in[0].texCrd + gs_in[2].texCrd.x;
	gs_out.texSlot = gs_in[0].texSlot;
	EmitVertex();

	EndPrimitive();
}

#shader_type shader
#version 400 core

out vec4 ps_pxClr;

in VS_OUT{
	vec4 vtxClr;
	vec2 texCrd;
	float texSlot;
} ps_in;

void main()
{
	ps_pxClr = vec4(texture(unf_tex[ps_in.texSlot], ps_in.texCrd).rgba *
		ps_in.vtxClr);
}