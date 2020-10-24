#version 330 core

out vec4 FragColor;

in vec2 vs_uvs;

struct Material {
	sampler2D texture_diffuse;
};

uniform vec3 lightColor;

uniform Material material;

void main()
{
	FragColor = vec4(texture(material.texture_diffuse, vs_uvs).rgba);
	//FragColor = vec4(0.05f, 0.3f, 0.4f, 0.5f);
}