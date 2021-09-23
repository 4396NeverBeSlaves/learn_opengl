#version 330 core

layout(location =0) in vec3 aPos;
layout(location =1) in vec2 aTexcoord;
layout(location =2) in vec3 aNormal;

out vec2 texcoord;
out vec3 normal;
out vec3 frag_world_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0);
	frag_world_pos=vec3(model*vec4(aPos,1.0));
	mat4 normal_matrix=transpose(inverse(model));
	normal=normalize(vec3(normal_matrix*vec4(aNormal,0.0)));
	texcoord=aTexcoord;
}