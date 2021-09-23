#version 330 core
//(x-3)^2+(y-3)^2+(z-3)^2=4;
layout(location =0) in vec3 aPos;

const vec3 sphere_center=vec3(0.0,0.0,0.0);
//const float radius =2.0;

uniform float x;
uniform float y;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_world_pos;
out vec3 normal;

void main(){

	gl_Position=projection*view*model*vec4(aPos,1.0);
	frag_world_pos=vec3(model*vec4(aPos,1.0));
	vec3 center_world_pos=vec3(model*vec4(sphere_center,1.0));
	normal=normalize(frag_world_pos-center_world_pos);
}