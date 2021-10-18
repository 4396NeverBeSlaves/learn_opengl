#version 330 core

uniform sampler2D blurTexture;
uniform bool horizontal;

in vec2 texcoord;

out vec4 FragColor;

void main(){
	vec2 offset=1.0/textureSize(blurTexture,0);
	const float coefs[]={0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216};
	vec3 result=texture(blurTexture,texcoord).rgb*coefs[0];

	if(horizontal){
		for(int i=1;i<5;i++){
			result+=texture(blurTexture,texcoord+vec2(offset.x*i,0.0)).rgb*coefs[i];
			result+=texture(blurTexture,texcoord-vec2(offset.x*i,0.0)).rgb*coefs[i];
		}
	}else{
		for(int i=1;i<5;i++){
			result+=texture(blurTexture,texcoord+vec2(0.0,offset.y*i)).rgb*coefs[i];
			result+=texture(blurTexture,texcoord-vec2(0.0,offset.y*i)).rgb*coefs[i];
		}
	}
	FragColor=vec4(result,1.0);
}