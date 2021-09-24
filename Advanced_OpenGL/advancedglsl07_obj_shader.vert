#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;
layout (std140) uniform myMatrices{
	mat4 view;
	mat4 projection;
};

//out vec2 texcoord;

out VS_OUT{
vec2 texcoord;
}vs_out;



uniform mat4 model;


void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0);
	vs_out.texcoord =aTexcoord;
}