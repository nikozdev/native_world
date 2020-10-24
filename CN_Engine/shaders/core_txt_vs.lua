#version 330 core

layout(location = 0) in vec2 m_coord;
layout(location = 1) in vec2 m_uvs;

out vec2 vs_uvs;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(m_coord.xy, 0.0f, 1.0f);
}