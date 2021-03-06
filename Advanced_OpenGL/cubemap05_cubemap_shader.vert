#version 330 core

layout(location =0) in vec3 aPos;
//layout(location =1) in vec2 aTexcoord;

out vec3 texcoord;

uniform mat4 view;
uniform mat4 projection;

void main(){
	vec4 position=projection*view*vec4(aPos,1.0);
	gl_Position=position.xyzz;
	texcoord=aPos;
}