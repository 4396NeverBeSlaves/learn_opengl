#version 330 core
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 light_pos;
	vec3 color;
//	float constant;
//	float linear;
//	float quadratic;
};

//in vec3 normal;
in vec3 frag_world_pos;
in vec2 texcoord;

uniform float time;
uniform vec3 eye_pos;
uniform Material object;
uniform Light light;

out vec4 FragColor;

float far=100.f;
float near=0.1f;

float viewz(float zn){
	float zc=zn*2.0f-1.0f;
	float zv=(2.0 * near * far) / (far + near - zc * (far - near));   
	return zv;
}

void main(){
	vec3 ambient=vec3(texture2D(object.diffuse,texcoord))*light.color;

//	vec3 light_dir=normalize(light.light_pos.xyz-frag_world_pos);
//	float _distance=length(light.light_pos.xyz-frag_world_pos);
//	float attenuation=1.0/(light.constant+light.linear*_distance+light.quadratic*_distance*_distance);
//
//	float diffuse_coef=max(dot(light_dir,normalize(normal)),0);
//	vec3 diffuse=diffuse_coef*vec3(texture2D(object.diffuse,texcoord))*light.color;
//
//	vec3 view_vec=normalize(eye_pos-frag_world_pos);
//	vec3 half_vec=normalize(light_dir+view_vec);
//	float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),object.shininess);
//	vec3 specular=specular_coef *vec3(texture2D(object.specular,texcoord))*light.color;
//
//	FragColor=vec4((ambient+diffuse+specular)*attenuation,1.0);

	//FragColor=vec4(ambient,1.0);
	float zv=(viewz(gl_FragCoord.z)-near)/(far-near);
	FragColor=vec4(vec3(zv),1.0);
}