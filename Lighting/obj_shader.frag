#version 330 core
in vec2 texCoord;

uniform vec3 obj_color;
uniform vec3 light_color;

//uniform float tranparency;
//uniform sampler2D myTexture0;
//uniform sampler2D myTexture1;

out vec4 FragColor;

void main(){
	//FragColor=mix(texture2D(myTexture0,texCoord),texture2D(myTexture1,texCoord),0.5f);
	
	//FragColor=texture2D(myTexture1,texCoord);

	FragColor=vec4(obj_color*light_color,1.0);
}