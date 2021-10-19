#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBitangents;

out vec3 frag_world_pos;
out vec2 texcoord;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0);
	frag_world_pos=vec3(model*vec4(aPos,1.0));
	texcoord=aTexcoord;

	mat4 normal_matrix=transpose(inverse(model));
	normal=mat3(normal_matrix)*aNormal;
}