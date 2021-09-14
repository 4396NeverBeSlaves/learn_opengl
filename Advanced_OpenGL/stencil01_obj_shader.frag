#version 330 core
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 light_pos;
	vec3 color;
};


in vec3 frag_world_pos;
in vec2 texcoord;

uniform Material object;
uniform Light light;

out vec4 FragColor;

void main(){
	vec3 ambient=vec3(texture2D(object.diffuse,texcoord))*light.color;
	FragColor=vec4(ambient,1.0);
}