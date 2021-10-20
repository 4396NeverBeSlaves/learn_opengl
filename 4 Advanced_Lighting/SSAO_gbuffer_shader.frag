#version 330 core
layout(location=0)out vec4 gPositionDepth;
layout(location=1)out vec3 gNormal;
layout(location=2)out vec4 gAlbedoSpecular;

struct Material{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	float shininess;
};

uniform Material material;

in vec3 frag_world_pos;
in vec2 texcoord;
in vec3 normal;

void main(){
	gPositionDepth.xyz=frag_world_pos;
	gPositionDepth.a=1.0;
	gNormal=normal;
	gAlbedoSpecular.rgb=texture(material.texture_diffuse0,texcoord).rgb;
	gAlbedoSpecular.a=texture(material.texture_specular0,texcoord).r;
}