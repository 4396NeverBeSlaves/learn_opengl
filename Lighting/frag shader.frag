#version 330 core
in vec3 color;
in vec2 texCoord;
//uniform vec3 color;

//uniform float tranparency;
uniform sampler2D myTexture0;
uniform sampler2D myTexture1;

out vec4 FragColor;

void main(){
	FragColor=mix(texture2D(myTexture0,texCoord),texture2D(myTexture1,texCoord),0.5f);
	
	//FragColor=texture2D(myTexture1,texCoord);

	//FragColor=vec4(1.0,0.0,0.0,1.0);
}