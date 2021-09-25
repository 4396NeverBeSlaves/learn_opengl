#version 330 core

layout(triangles) in;
layout(line_strip,max_vertices=6) out;

in VS_OUT{
	vec2 texcoord;
	vec3 normal;
	vec3 frag_world_pos;
}gs_in[];

in mat4 projection_geom[];
in mat4 view_geom[];

out VS_OUT{
	vec2 texcoord;
	vec3 normal;
	vec3 frag_world_pos;
}gs_out;

out vec4 color;

void gen_normal(int i){
	gs_out.texcoord=gs_in[i].texcoord;
	gs_out.normal=gs_in[i].normal;
	gs_out.frag_world_pos=gs_in[i].frag_world_pos;
	gl_Position=gl_in[i].gl_Position;
	EmitVertex();
	gl_Position=projection_geom[i]*view_geom[i]*vec4(gs_in[i].frag_world_pos+gs_in[i].normal,1.0);
	EmitVertex();

	EndPrimitive();
}

void main(){
	color=vec4(1.0,0.0,0.0,1.0);
	gen_normal(0);

	color=vec4(0.0,1.0,0.0,1.0);
	gen_normal(1);

	color=vec4(0.0,0.0,1.0,1.0);
	gen_normal(2);
}