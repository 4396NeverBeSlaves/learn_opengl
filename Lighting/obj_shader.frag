#version 330 core
in vec3 normal;
in vec3 world_pos;

uniform vec3 ambient;
uniform vec3 obj_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 eye_pos;

out vec4 FragColor;

void main(){

	vec3 light_dir=normalize(light_pos-world_pos);
	float diffuse_coef=max(dot(light_dir,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*light_color;

	vec3 view_vec=normalize(eye_pos-world_pos);
	vec3 half_vec=normalize(light_dir+view_vec);
	float specular_coef=max(dot(half_vec,normalize(normal)),0);
	vec3 specular=pow(specular_coef,1024) *light_color;

	FragColor=vec4(obj_color*(ambient+diffuse+specular),1.0);
}