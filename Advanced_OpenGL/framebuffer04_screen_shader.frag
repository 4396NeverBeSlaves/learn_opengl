#version 330 core

uniform sampler2D screen_texture;

in vec2 texcoord;

out vec4 FragColor;

void main(){
	FragColor=vec4(texture2D(screen_texture,texcoord).rgb,1.0);
	
}