#version 330 core

uniform sampler2D screenTexture;
uniform sampler2D blurTexture;
uniform bool use_gamma;
uniform bool use_hdr;
uniform float exposure;
uniform bool use_bloom;

in vec2 texcoord;

out vec4 FragColor;

void main(){
	const float gamma=2.2;
	vec3 result;
	vec3 mapping_color;

	vec3 sampling_color=vec3(texture(screenTexture,texcoord));
	
	if(use_bloom){
		sampling_color+=texture(blurTexture,texcoord).rgb;
	}

	if(use_hdr){
		mapping_color=vec3(1.0)-exp(-sampling_color*exposure);
	}else{
		mapping_color=sampling_color;
	}
	


	if(use_gamma){
		result=pow(mapping_color,vec3(1.0/gamma));
	}else{
		result=mapping_color;
	}
	FragColor=vec4(result,1.0);
}