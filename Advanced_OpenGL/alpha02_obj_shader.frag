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
	vec4 outcolor=texture2D(object.diffuse,texcoord);
	if(outcolor.a<0.1)
		discard;
	FragColor=outcolor*vec4(light.color,1.0);
}