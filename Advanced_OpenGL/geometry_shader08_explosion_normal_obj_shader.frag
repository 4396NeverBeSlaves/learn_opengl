#version 330 core

uniform sampler2D diffusemap;
uniform samplerCube cubemap;
uniform vec3 eye_pos;

in VS_OUT{
	vec2 texcoord;
	vec3 normal;
	vec3 frag_world_pos;
}fs_in;

out vec4 FragColor;

void main(){
	vec3 view_dir=normalize(eye_pos-fs_in.frag_world_pos);
	vec3 reflect_dir=reflect(-view_dir,fs_in.normal);

	FragColor=mix(texture(diffusemap,fs_in.texcoord),texture(cubemap,reflect_dir),0.5);
}