#version 330 core
layout(location=0)out vec3 gPosition;
layout(location=1)out vec3 gNormal;
layout(location=2)out vec4 gAlbedoSpecular;

uniform vec3 light_color;

in vec3 frag_world_pos;
in vec3 normal;

void main(){
	gPosition=frag_world_pos;
	gNormal=normalize(normal);
	gAlbedoSpecular=vec4(light_color,1.0);
}