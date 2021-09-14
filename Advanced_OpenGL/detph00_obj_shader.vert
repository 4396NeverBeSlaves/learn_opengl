#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
layout (location = 1) in vec2 aTexcoord;

//out vec3 normal;
//out vec3 frag_world_pos;
out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 normal_matrix_translate;
//uniform mat4 normal_matrix_rotate;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0);
	//frag_world_pos=vec3(model*vec4(aPos,1.0));
	//normal=(normal_matrix_translate*normal_matrix_rotate* vec4(aNormal,0.0)).xyz;
	texcoord=aTexcoord;
}