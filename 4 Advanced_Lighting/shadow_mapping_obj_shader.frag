#version 330 core

struct Material{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
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
in vec4 lighting_position;

#define NUMS_POINT_LIGHTS 1
uniform float time;
uniform vec3 eye_pos;
uniform Material material;
uniform DirectionLight dir_light;
uniform PointLight point_lights[NUMS_POINT_LIGHTS];
uniform SpotLight spot_light;

uniform sampler2D shadowmap;

out vec4 FragColor;

float calc_shadow(vec4 lighting_position,vec3 light_dir,vec3 normal){
	vec3 lighting_pos=lighting_position.xyz/lighting_position.w;
	lighting_pos=(lighting_pos+1.0)*0.5;
//	float real_depth=texture(shadowmap,vec2(lighting_pos.x,lighting_pos.y)).r;
//	float diff=lighting_pos.z-real_depth;
//	//return lighting_pos.z>real_depth?1.0:0.0;
//
//	//float delta=max(0.002*(1.0-dot(light_dir,normal)),0.001);
//	float delta=0.001;
//	return diff>delta?1.0:0.0;

	vec2 offset=1.0/textureSize(shadowmap,0);
	float shadow=0.0;
	float bias=0.002;

	for(int y=-1;y<=1;y++){
		for(int x=-1;x<=1;x++){
			float single_depth=texture(shadowmap,vec2(lighting_pos.x,lighting_pos.y)+vec2(x*offset.x,y*offset.y)).r;
			shadow+= single_depth+bias<lighting_pos.z?1.0:0.0;
		}
	}

	shadow=shadow/9.0;
	return shadow;
}

vec3 cal_direction_light(DirectionLight light){
	

	vec3 ambient=vec3(texture2D(material.texture_diffuse0,texcoord))*light.color*0.5f;
	vec3 direction=normalize(-light.direction);
	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(material.texture_diffuse0,texcoord))*light.color;

	vec3 view_vec=normalize(eye_pos-frag_world_pos);
	vec3 half_vec=normalize(direction+view_vec);
	vec3 specular;
	if(material.shininess>=1.0){
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),material.shininess);
		specular=specular_coef *vec3(texture2D(material.texture_specular0,texcoord))*light.color;
	}else{
		specular=vec3(0.0);
	}

	float shadow=calc_shadow(lighting_position,direction,normal);

	return ambient+(1.0-shadow)*(diffuse+specular);
}

vec3 cal_point_light(PointLight light){
	vec3 ambient=vec3(texture2D(material.texture_diffuse0,texcoord))*light.color*0.2f;
	
	vec3 direction=normalize(light.position-frag_world_pos);
	float distance_=length(light.position.xyz-frag_world_pos);
	float attenuation=1.0/(light.constant+light.linear*distance_+light.quadratic*distance_*distance_);

	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(material.texture_diffuse0,texcoord))*light.color;

	vec3 view_vec=normalize(eye_pos-frag_world_pos);
	vec3 half_vec=normalize(direction+view_vec);
	vec3 specular;
	if(material.shininess>=1.0){
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),material.shininess);
		specular=specular_coef *vec3(texture2D(material.texture_specular0,texcoord))*light.color;
	}else{
		specular=vec3(0.0);
	}

	return (ambient+diffuse+specular)*attenuation;
}
vec3 cal_spot_light(SpotLight light){
	vec3 ambient=vec3(texture2D(material.texture_diffuse0,texcoord))*light.color*0.2f;

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

	result+=cal_direction_light(dir_light);
//	for(int i=0;i<NUMS_POINT_LIGHTS;i++){
//		result+=cal_point_light(point_lights[i]);
//	}
	//result+=cal_spot_light(spot_light);
	//vec3 emission=texture2D(material.emission,texcoord).rgb;
	vec3 emission=vec3(0.0);
	//result=pow(result,vec3(1.0/2.2));
	FragColor=vec4(result+emission,1.0);

}