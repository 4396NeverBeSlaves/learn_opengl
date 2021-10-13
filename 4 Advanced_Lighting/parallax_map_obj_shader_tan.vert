#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBitangents;

out VS_OUT{
	vec3 frag_pos_tan;
	vec3 eye_pos_tan;
	vec2 texcoord;
	mat3 TBN_inverse;
}vs_out;

uniform vec3 eye_pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0);
	vec3 frag_world_pos=vec3(model*vec4(aPos,1.0));
	vs_out.texcoord=aTexcoord;

	mat4 normal_matrix=transpose(inverse(model));

	vec3 tangents=normalize(mat3(model)* aTangents);
	vec3 bitangents=normalize(mat3(model)* aBitangents);
	vec3 n=cross(tangents,bitangents);
	vs_out.TBN_inverse=transpose(mat3(tangents,bitangents,n));
	vs_out.frag_pos_tan=vs_out.TBN_inverse*frag_world_pos;
	vs_out.eye_pos_tan=vs_out.TBN_inverse*eye_pos;
}