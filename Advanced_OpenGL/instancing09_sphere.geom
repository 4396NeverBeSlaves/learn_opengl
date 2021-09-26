#version 330 core

layout(triangles) in;
layout(triangle_strip,max_vertices=3) out;

uniform float time;

in VS_OUT{
	vec3 normal;
	vec3 frag_world_pos;
}gs_in[];

out VS_OUT{
	vec3 normal;
	vec3 frag_world_pos;
}gs_out;

in mat4 view_geom[];
in mat4 proj_geom[];

void main(){	
	vec3 v1=vec3(gs_in[1].frag_world_pos-gs_in[0].frag_world_pos);
	vec3 v2=vec3(gs_in[2].frag_world_pos-gs_in[0].frag_world_pos);	
	vec3 normal=normalize(cross(v1,v2))*((sin(radians(time*100.0))+1.0)/2.0);

	gs_out.normal=gs_in[0].normal;
	gs_out.frag_world_pos=gs_in[0].frag_world_pos;
	gl_Position=proj_geom[0]*view_geom[0]*(vec4(gs_in[0].frag_world_pos,1.0)+vec4(normal,0.0));
	EmitVertex();

	gs_out.normal=gs_in[1].normal;
	gs_out.frag_world_pos=gs_in[1].frag_world_pos;
	gl_Position=proj_geom[0]*view_geom[0]*(vec4(gs_in[1].frag_world_pos,1.0)+vec4(normal,0.0));
	EmitVertex();

	gs_out.normal=gs_in[2].normal;
	gs_out.frag_world_pos=gs_in[2].frag_world_pos;
	gl_Position=proj_geom[0]*view_geom[0]*(vec4(gs_in[2].frag_world_pos,1.0)+vec4(normal,0.0));
	EmitVertex();
	EndPrimitive();
}