#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_world_pos;
out vec3 normal;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0);
	frag_world_pos=(model*vec4(aPos,1.0)).xyz;
	normal= transpose(inverse(mat3(model)))*aNormal;
}
