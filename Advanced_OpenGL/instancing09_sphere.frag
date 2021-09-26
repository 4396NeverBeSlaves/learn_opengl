#version 330 core

uniform samplerCube cubemap;
uniform vec3 eye_pos;

in VS_OUT{
	vec3 normal;
	vec3 frag_world_pos;
}fs_in;


out vec4 FragColor;

void main(){
	vec3 view_dir=normalize(eye_pos-fs_in.frag_world_pos);
	vec3 reflect_dir=reflect(-view_dir,fs_in.normal);
	FragColor=texture(cubemap,reflect_dir);
	//FragColor=vec4(1.0,0.0,0.0,1.0);
}