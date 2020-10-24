#version 330 core

layout(location = 0) in vec2 m_pos;
layout(location = 1) in vec2 m_uvs;

uniform mat4 model;
uniform mat4 projView;

out vec3 vs_pos;
out vec2 vs_uvs;

void main()
{
	vs_uvs = m_uvs;
	vs_pos = vec3(model * vec4(m_pos, 0.0f, 1.0f));
	//gl_Position = model * projView * vec4(m_pos, 0.0f, 1.0f);
	gl_Position = vec4(m_pos, 0.0f, 1.0f);
}