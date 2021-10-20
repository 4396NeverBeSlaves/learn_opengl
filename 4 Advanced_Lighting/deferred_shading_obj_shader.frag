#version 330 core
layout(location=0)out vec4 FragColor;

struct Material{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	float shininess;
};

struct DirectionLight{
	vec3 color;
	vec3 direction;
};

struct PointLight{
	vec3 color;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 color;
	vec3 position;
	vec3 spot_dir;
	float inner_range_angle;
	float outer_range_angle;
	float constant;
	float linear;
	float quadratic;
};

in vec3 frag_world_pos;
in vec2 texcoord;
in vec3 normal;

#define MAX_DIRECTION_LIGHTS_NUM 16
#define MAX_POINT_LIGHTS_NUM 68
#define MAX_SPOT_LIGHTS_NUM 16

uniform float time;
uniform vec3 eye_pos;
uniform Material material;
uniform ivec3 lights_count;
uniform DirectionLight dir_lights[MAX_DIRECTION_LIGHTS_NUM];
uniform PointLight point_lights[MAX_POINT_LIGHTS_NUM];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS_NUM];


vec3 cal_direction_light(DirectionLight light){
	vec3 view_vec=normalize(eye_pos-frag_world_pos);

	vec3 ambient=vec3(texture2D(material.texture_diffuse0,texcoord))*light.color*0.2f;
	vec3 direction=normalize(-light.direction);
	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(material.texture_diffuse0,texcoord))*light.color;
	diffuse=vec3(0.0);

	vec3 half_vec=normalize(direction+view_vec);
	vec3 specular;
	if(material.shininess>=1.0){
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),material.shininess);
		specular=specular_coef *vec3(texture2D(material.texture_diffuse0,texcoord))*light.color;

	}else{
		specular=vec3(0.0);
	}

	return ambient+diffuse+specular;
}

vec3 cal_point_light(PointLight light){
	vec3 view_vec=normalize(eye_pos-frag_world_pos);

	vec3 ambient=vec3(texture2D(material.texture_diffuse0,texcoord))*light.color*0.2f;

	vec3 direction=normalize(light.position-frag_world_pos);
	float distance_=length(light.position.xyz-frag_world_pos);
	float attenuation=1.0/(light.constant+light.linear*distance_+light.quadratic*distance_*distance_);

	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(material.texture_diffuse0,texcoord))*light.color;

	vec3 specular;
	if(material.shininess>=1.0){
		
		vec3 half_vec=normalize(direction+view_vec);
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),material.shininess);
		specular=specular_coef *vec3(texture2D(material.texture_specular0,texcoord))*light.color;

	}else{
		specular=vec3(0.0);
	}

	return (ambient+diffuse+specular)*attenuation;
}
vec3 cal_spot_light(SpotLight light){
	vec3 view_vec=normalize(eye_pos-frag_world_pos);

	vec3 ambient=vec3(texture2D(material.texture_diffuse0,texcoord))*light.color*0.2f;

	vec3 direction=normalize(light.position.xyz-frag_world_pos);

//	if(dot(view_vec,normalize(normal))<0.0)	//只有当观察向量、灯光向量和法向量在同一平面方向时光才可见
//		return vec3(0.0);
//	if(dot(direction,normalize(normal))<0.0)
//		return vec3(0.0);
	float distance_=distance(light.position.xyz,frag_world_pos);
	float attenuation=1.0/(light.constant+light.linear*distance_+light.quadratic*distance_*distance_);

	float in_light=0.0;

	float inner_range=cos(radians(light.inner_range_angle));
	float outer_range=cos(radians(light.outer_range_angle));
	float epsilon=inner_range-outer_range;
	float result=dot(-direction,normalize(light.spot_dir));
	in_light=clamp((result-outer_range)/epsilon,0.0,1.0);

	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(material.texture_diffuse0,texcoord))*light.color;

	
	vec3 half_vec=normalize(direction+view_vec);
	vec3 specular;
	if(material.shininess>=1.0){
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),material.shininess);
		specular=specular_coef *vec3(texture2D(material.texture_specular0,texcoord))*light.color;
	}else{
		specular=vec3(0.0);
	}

	return (ambient+diffuse+specular)*attenuation*in_light;
}
void main(){
	vec3 result=vec3(0.0);

	for(int i=0;i<lights_count[0];i++){
		result+=cal_direction_light(dir_lights[i]);
	}
	for(int i=0;i<lights_count[1];i++){
		result+=cal_point_light(point_lights[i]);
	}
	for(int i=0;i<lights_count[2];i++){
		result+=cal_spot_light(spot_lights[i]);
	}

	FragColor=vec4(result,1.0);
}