#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

out vec3 normal;
out vec3 frag_world_pos;
out vec2 texcoord;
out vec4 lighting_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lighting_matrix;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0);
	frag_world_pos=vec3(model*vec4(aPos,1.0));
	mat4 normal_matrix=transpose(inverse(model));
	normal=(normal_matrix* vec4(aNormal,0.0)).xyz;
	texcoord=aTexcoord;
	lighting_position=lighting_matrix*vec4(frag_world_pos,1.0);
}