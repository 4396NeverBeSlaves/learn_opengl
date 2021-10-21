#version 330 core

uniform sampler2D ssaoTex;

in vec2 texcoord;

out float FragColor;
void main(){
	float result=0.0;
	vec2 texture_scale=vec2(1.0)/textureSize(ssaoTex,0);
	for(int i=-2;i<2;i++){
		for(int j=-2;j<2;j++){
			result+=texture(ssaoTex,texcoord+vec2(i,j)*texture_scale).r;
		}
	}
	result/=16.0;
	FragColor=result;
}
