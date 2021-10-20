#version 330 core

uniform vec3 light_color;

in vec3 frag_world_pos;
in vec3 normal;

out vec4 FragColor;

void main(){
	FragColor=vec4(light_color,1.0);
}