#version 330 core

layout(location =0) in vec3 aPos;
layout(location =1) in vec3 aOffset;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out VS_OUT{
	vec3 normal;
	vec3 frag_world_pos;
}vs_out;

out mat4 view_geom;
out mat4 proj_geom;

void main(){
	vec3 position=aPos+aOffset;
	gl_Position=projection*view*model*vec4(position,1.0);
	vs_out.frag_world_pos=vec3(model*vec4(position,1.0));
	vec3 center_world_pos=vec3(model*vec4(aOffset,1.0));
	vs_out.normal=normalize(vs_out.frag_world_pos-center_world_pos);
	view_geom=view;
	proj_geom=projection;
}