#shader_type vertex
#version 400 core

layout(location = 1) in vec2 atb_texCrd;

out VS_OUT {
	vec2 texCrd;
} vs_out;

void main() {
	gl_Position = vec4(atb_vtxCrd.xy, 0.0f, 1.0f);
	
	vs_out.texCrd = atb_texCrd;
}

#shader_type pixel
#version 400 core

out vec4 ps_pxClr;

in VS_OUT {
	vec2 texCrd;
} vs_in;

uniform sampler2D unf_texture0;

void main() {
	ps_pxClr = texture(unf_texture0, vs_in.texCrd).rgba;
}