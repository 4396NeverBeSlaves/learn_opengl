#version 330 core

//in vec3 frag_world_pos;
//in vec2 texcoord;

in VS_OUT{
vec2 texcoord;
}fs_in;

uniform sampler2D diffusemap;
uniform sampler2D diffusemap1;

out vec4 FragColor;

void main(){
//	if(gl_FragCoord.x<450&&gl_FragCoord.y<450){
//		vec3 ambient=vec3(texture2D(object.diffuse,texcoord));
//		FragColor=vec4(ambient,1.0);
//	}	
//	if(gl_FragCoord.x>=450&&gl_FragCoord.y<450){
//		FragColor=vec4(1.0,0.0,0.0,1.0);
//	}	
//	if(gl_FragCoord.x<450&&gl_FragCoord.y>=450){
//		FragColor=vec4(0.0,1.0,0.0,1.0);
//	}	
//	if(gl_FragCoord.x>=450&&gl_FragCoord.y>=450){
//		FragColor=vec4(0.0,0.0,1.0,1.0);
//	}
	if(gl_FrontFacing){
		FragColor=vec4(vec3(texture2D(diffusemap,fs_in.texcoord)),1.0);
	}else{
		FragColor=vec4(vec3(texture2D(diffusemap1,fs_in.texcoord)),1.0);
	}
	
}