#version 330 core
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 light_pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec3 frag_world_pos;
in vec2 texcoord;

uniform vec3 eye_pos;
uniform Material m_gold;
uniform Light light;


out vec4 FragColor;

void main(){
	vec3 ambient=vec3(texture2D(m_gold.diffuse,texcoord))*light.ambient;

	vec3 light_dir=normalize(light.light_pos-frag_world_pos);
	float diffuse_coef=max(dot(light_dir,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(m_gold.diffuse,texcoord))*light.diffuse;

	vec3 view_vec=normalize(eye_pos-frag_world_pos);
	vec3 half_vec=normalize(light_dir+view_vec);
	float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),m_gold.shininess);
	vec3 specular=specular_coef *vec3(texture2D(m_gold.specular,texcoord))*light.specular;
	
	FragColor=vec4(ambient+diffuse+specular,1.0);

}