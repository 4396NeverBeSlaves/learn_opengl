#version 330 core

#define SAMPLING_TIMES 32

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D noises;
uniform mat4 projection;
uniform vec3 sampling_kernel[SAMPLING_TIMES];
uniform bool smooth_edge;
uniform float sampling_radius;

in vec2 texcoord;

out float FragColor;

const int SCR_WIDTH=900;
const int SCR_HEIGHT=900;


void main(){
	vec3 frag_view_pos=texture(gPosition,texcoord).xyz;
	vec3 normal=normalize(texture(gNormal,texcoord).xyz);

	vec2 noise_scale=vec2(SCR_WIDTH*1.0,SCR_HEIGHT*1.0)/textureSize(noises,0);
	vec3 random_rotate=texture(noises,texcoord*vec2(225.0,225.0)).xyz;

	vec3 tangent=normalize(random_rotate-normal*dot(normal,random_rotate));
	vec3 bitangent=cross(normal,tangent);
	mat3 TBN=mat3(tangent,bitangent,normal);

	float occlusion=0.0;

	for(int i=0;i<SAMPLING_TIMES;i++){
		vec3 sample_view_pos=frag_view_pos+TBN*sampling_kernel[i]*sampling_radius;

		vec4 sample_screen_pos=projection*vec4(sample_view_pos,1.0);
		sample_screen_pos.xy/=sample_screen_pos.w;
		sample_screen_pos.xy=sample_screen_pos.xy*0.5+0.5;

		float real_depth=texture(gPosition,sample_screen_pos.xy).z;
		float range_check;
		if(smooth_edge){
			range_check=smoothstep(0.8,1.0,sampling_radius/abs(real_depth-frag_view_pos.z));
		}else{
			range_check=abs(real_depth-frag_view_pos.z)<=sampling_radius?1.0:0.0;
		}
		occlusion+=(real_depth>=sample_view_pos.z+0.01?1.0:0.0)*range_check;
	}
	occlusion=1.0-occlusion/SAMPLING_TIMES;

	FragColor=occlusion;
}