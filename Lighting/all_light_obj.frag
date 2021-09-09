#version 330 core

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
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

in vec3 normal;
in vec3 frag_world_pos;
in vec2 texcoord;

#define NUMS_POINT_LIGHTS 4
uniform float time;
uniform vec3 eye_pos;
uniform Material wood_box;
uniform DirectionLight dir_light;
uniform PointLight point_lights[NUMS_POINT_LIGHTS];
uniform SpotLight spot_light;

out vec4 FragColor;

vec3 cal_direction_light(DirectionLight light){

	vec3 ambient=vec3(texture2D(wood_box.diffuse,texcoord))*light.color*0.2f;
	vec3 direction=normalize(-light.direction);
	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(wood_box.diffuse,texcoord))*light.color;

	vec3 view_vec=normalize(eye_pos-frag_world_pos);
	vec3 half_vec=normalize(direction+view_vec);
	float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),wood_box.shininess);
	vec3 specular=specular_coef *vec3(texture2D(wood_box.specular,texcoord))*light.color;

	return ambient+diffuse+specular;
}

vec3 cal_point_light(PointLight light){
	vec3 ambient=vec3(texture2D(wood_box.diffuse,texcoord))*light.color*0.2f;
	
	vec3 direction=normalize(light.position-frag_world_pos);
	float distance_=length(light.position.xyz-frag_world_pos);
	float attenuation=1.0/(light.constant+light.linear*distance_+light.quadratic*distance_*distance_);

	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(wood_box.diffuse,texcoord))*light.color;

	vec3 view_vec=normalize(eye_pos-frag_world_pos);
	vec3 half_vec=normalize(direction+view_vec);
	float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),wood_box.shininess);
	vec3 specular=specular_coef *vec3(texture2D(wood_box.specular,texcoord))*light.color;
	
	return (ambient+diffuse+specular)*attenuation;
}
vec3 cal_spot_light(SpotLight light){
	vec3 ambient=vec3(texture2D(wood_box.diffuse,texcoord))*light.color*0.2f;

	vec3 direction=normalize(light.position.xyz-frag_world_pos);
	vec3 view_vec=normalize(eye_pos-frag_world_pos);

	if(dot(view_vec,normalize(normal))<0.0)	//只有当观察向量、灯光向量和法向量在同一平面方向时光才可见
		return vec3(0.0);
	if(dot(direction,normalize(normal))<0.0)
		return vec3(0.0);
	float distance_=distance(light.position.xyz,frag_world_pos);
	float attenuation=1.0/(light.constant+light.linear*distance_+light.quadratic*distance_*distance_);

	float in_light=0.0;

	float inner_range=cos(radians(light.inner_range_angle));
	float outer_range=cos(radians(light.outer_range_angle));
	float epsilon=inner_range-outer_range;
	float result=dot(-direction,normalize(light.spot_dir));
/*//	if(result<=1.0 && result >=range)
//		in_light=1.0;
//	else
	in_light=pow((1.0-(max(range-result,0))),100);*/
	in_light=clamp((result-outer_range)/epsilon,0.0,1.0);

	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(wood_box.diffuse,texcoord))*light.color;

	
	vec3 half_vec=normalize(direction+view_vec);
	float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),wood_box.shininess);
	vec3 specular=specular_coef *vec3(texture2D(wood_box.specular,texcoord))*light.color;

	return (ambient+diffuse+specular)*attenuation*in_light;
}
void main(){
	vec3 result=vec3(0.0);

	result+=cal_direction_light(dir_light);
	for(int i=0;i<NUMS_POINT_LIGHTS;i++){
		result+=cal_point_light(point_lights[i]);
	}
	result+=cal_spot_light(spot_light);
	//vec3 emission=texture2D(wood_box.emission,texcoord).rgb;
	vec3 emission=vec3(0.0);

	FragColor=vec4(result+emission,1.0);

}