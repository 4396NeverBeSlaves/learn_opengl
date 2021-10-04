#version 330 core

uniform sampler2D shadowmap;

in vec2 texcoord;

out vec4 FragColor;

void main(){
	float color=texture(shadowmap,texcoord).r;
	FragColor=vec4(vec3(color),1.0);
}