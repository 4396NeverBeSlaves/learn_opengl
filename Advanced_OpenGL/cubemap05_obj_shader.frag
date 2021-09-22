#version 330 core

uniform sampler2D diffusemap;

in vec2 texcoord;

out vec4 FragColor;

void main(){
	FragColor=texture(diffusemap,texcoord);
}