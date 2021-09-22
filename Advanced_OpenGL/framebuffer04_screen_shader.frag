#version 330 core

uniform sampler2D screen_texture;

in vec2 texcoord;

out vec4 FragColor;

void main(){
//	vec3 res=vec3(0.0);
//	float offset=1.0/400.0;
//	vec2 offsets[]={
//		vec2(-offset,offset),
//		vec2(0.0,offset),
//		vec2(offset,offset),
//		vec2(-offset,0.0),
//		vec2(0.0,0.0),
//		vec2(offset,0.0),
//		vec2(-offset,-offset),
//		vec2(0.0,-offset),
//		vec2(offset,-offset)
//	};
//
//	float kernel[]={
//		2, 2, 2,
//        2, -16,2,
//        2, 2,2
//	};
//
//	for( int i=0;i<9;i++){
//		
//		vec2 sample_coord=texcoord+offsets[i];
//		res+=vec3(texture2d(screen_texture,sample_coord))*kernel[i];
//	}
	vec3 res=texture2D(screen_texture,texcoord).rgb;
	FragColor=vec4( res,1.0);
	
}