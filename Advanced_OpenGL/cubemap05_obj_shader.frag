#version 330 core

uniform sampler2D diffusemap;
uniform samplerCube cubemap;
uniform vec3 eye_pos;

in vec2 texcoord;
in vec3 normal;
in vec3 frag_world_pos;

out vec4 FragColor;

void main(){
	vec3 view_dir=normalize(eye_pos-frag_world_pos);
	//vec3 reflect_dir=reflect(-view_dir,normal);
	float ratio=1.00/1.52;
	vec3 refract_dir=refract(-view_dir,normal,ratio);

	FragColor=mix(texture(diffusemap,texcoord),texture(cubemap,refract_dir),1.0);
}