#version 330 core

//(x-3)^2+(y-3)^2+(z-3)^2=4;

//const vec3 sphere_center=vec3(3.0,3.0,3.0);
//const float radius =2.0;

uniform samplerCube cubemap;
uniform vec3 eye_pos;

in vec3 frag_world_pos;
in vec3 normal;

out vec4 FragColor;

void main(){
	vec3 view_dir=normalize(eye_pos-frag_world_pos);
	float ratio=1.00/1.52;
	vec3 refract_dir=refract(-view_dir,normal,ratio);
	//vec3 refract_dir=reflect(-view_dir,normal);
	FragColor=texture(cubemap,refract_dir);
//FragColor=vec4(1.0,0.0,0.0,1.0);
}