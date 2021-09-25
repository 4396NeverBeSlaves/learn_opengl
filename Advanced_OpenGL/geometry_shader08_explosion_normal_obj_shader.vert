#version 330 core

layout(location =0) in vec3 aPos;
layout(location =1) in vec2 aTexcoord;
layout(location =2) in vec3 aNormal;

out VS_OUT{
	vec2 texcoord;
	vec3 normal;
	vec3 frag_world_pos;
}vs_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out mat4 projection_geom;
out mat4 view_geom;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0);
	vs_out.frag_world_pos=vec3(model*vec4(aPos,1.0));
	mat4 normal_matrix=transpose(inverse(model));
	vs_out.normal=normalize(vec3(normal_matrix*vec4(aNormal,0.0)));
	vs_out.texcoord=aTexcoord;

	projection_geom=projection;
	view_geom=view;
}