#version 330 core

#define MAX_DIRECTION_LIGHTS_NUM 16
#define MAX_POINT_LIGHTS_NUM 68
#define MAX_SPOT_LIGHTS_NUM 16

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


uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpecular;
uniform sampler2D ssaoTexture;
uniform sampler2D ssao_blur_Texture;
uniform DirectionLight dir_lights[MAX_DIRECTION_LIGHTS_NUM];
uniform PointLight point_lights[MAX_POINT_LIGHTS_NUM];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS_NUM];
uniform ivec3 lights_count;
uniform bool use_gamma;
uniform bool use_hdr;
uniform bool use_AO;
uniform float exposure;
uniform bool check_texture;
uniform int texture_id;
uniform mat4 view_matrix;
uniform vec3 eye_pos;

in vec2 texcoord;

out vec4 FragColor;

const float shininess=96.078431;

vec3 cal_direction_light(DirectionLight light){
	vec3 frag_view_pos=texture(gPositionDepth,texcoord).xyz;
	vec3 normal=texture(gNormal,texcoord).xyz;
	vec4 albedo_spec=texture(gAlbedoSpecular,texcoord);
	vec3 albedo=albedo_spec.rgb;
	vec3 spec=albedo_spec.aaa;

	vec3 view_vec=normalize(eye_pos-frag_view_pos);

	vec3 ambient=albedo*light.color*0.2f;
	vec3 direction=normalize(-light.direction);
	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*albedo*light.color;

	vec3 half_vec=normalize(direction+view_vec);
	vec3 specular;
	if(shininess>=1.0){
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),shininess);
		specular=specular_coef *spec*light.color;

	}else{
		specular=vec3(0.0);
	}

	return ambient+diffuse+specular;
}

vec3 cal_point_light(PointLight light){
	vec3 frag_view_pos=texture(gPositionDepth,texcoord).xyz;
	vec3 normal=texture(gNormal,texcoord).xyz;
	vec4 albedo_spec=texture(gAlbedoSpecular,texcoord);
	vec3 albedo=albedo_spec.rgb;
	vec3 spec=albedo_spec.aaa;
	vec3 light_view_pos=(view_matrix*vec4(light.position,1.0)).xyz;
	vec3 eye_view_pos=(view_matrix*vec4(eye_pos,1.0)).xyz;

	vec3 view_vec=normalize(eye_view_pos-frag_view_pos);

	vec3 ambient;
	if(use_AO)
		ambient=albedo*light.color*vec3(texture(ssao_blur_Texture,texcoord).r)*0.2f;
	else
		ambient= albedo*light.color*0.2f;

	vec3 direction=normalize(light_view_pos-frag_view_pos);
	float distance_=length(light_view_pos.xyz-frag_view_pos);
	float attenuation=1.0/(light.constant+light.linear*distance_+light.quadratic*distance_*distance_);

	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*albedo*light.color;

	vec3 specular;
	vec3 half_vec=normalize(direction+view_vec);
	float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),shininess);
	specular=specular_coef *spec*light.color;


	return (ambient+diffuse+specular)*attenuation;
}
vec3 cal_spot_light(SpotLight light){
	vec3 frag_view_pos=texture(gPositionDepth,texcoord).xyz;
	vec3 normal=texture(gNormal,texcoord).xyz;
	vec4 albedo_spec=texture(gAlbedoSpecular,texcoord);
	vec3 albedo=albedo_spec.rgb;
	vec3 spec=albedo_spec.aaa;
	vec3 light_view_pos=(view_matrix*vec4(light.position,1.0)).xyz;
	vec3 light_view_spotdir=(view_matrix*vec4(light.spot_dir,0.0)).xyz;

	vec3 view_vec=normalize(eye_pos-frag_view_pos);

	vec3 ambient=albedo*light.color*0.2f;

	vec3 direction=normalize(light_view_pos.xyz-frag_view_pos);

//	if(dot(view_vec,normalize(normal))<0.0)	//只有当观察向量、灯光向量和法向量在同一平面方向时光才可见
//		return vec3(0.0);
//	if(dot(direction,normalize(normal))<0.0)
//		return vec3(0.0);
	float distance_=distance(light_view_pos.xyz,frag_view_pos);
	float attenuation=1.0/(light.constant+light.linear*distance_+light.quadratic*distance_*distance_);

	float in_light=0.0;

	float inner_range=cos(radians(light.inner_range_angle));
	float outer_range=cos(radians(light.outer_range_angle));
	float epsilon=inner_range-outer_range;
	float result=dot(-direction,normalize(light_view_spotdir));
	in_light=clamp((result-outer_range)/epsilon,0.0,1.0);

	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*albedo*light.color;

	
	vec3 half_vec=normalize(direction+view_vec);
	vec3 specular;
	if(shininess>=1.0){
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),shininess);
		specular=specular_coef *spec*light.color;
	}else{
		specular=vec3(0.0);
	}

	return (ambient+diffuse+specular)*attenuation*in_light;
}
void main(){
	const float gamma=2.2;
	vec3 result;

	vec4 light_model=texture(gAlbedoSpecular,texcoord);
	if(light_model.a==-1.0){
		FragColor=vec4(light_model.rgb,1.0);
		return;
	}

	if(check_texture){
		if(texture_id==0){
			FragColor=vec4(texture(ssaoTexture,texcoord).rrr,1.0);
			return;
		}else if(texture_id==1){
			FragColor=vec4(texture(ssao_blur_Texture,texcoord).rrr,1.0);
			return;
		}
		else if(texture_id==2){
			FragColor=vec4(texture(gPositionDepth,texcoord).rgb,1.0);
			return;
		}else if(texture_id==3){
			FragColor=texture(gNormal,texcoord);
			return;
		}else if(texture_id==4){
			FragColor=texture(gAlbedoSpecular,texcoord);
			return;
		}
	}

	for(int i=0;i<lights_count[0];i++){
		result+=cal_direction_light(dir_lights[i]);
	}
	for(int i=0;i<lights_count[1];i++){
		result+=cal_point_light(point_lights[i]);
	}
	for(int i=0;i<lights_count[2];i++){
		result+=cal_spot_light(spot_lights[i]);
	}

	if(use_hdr)
		result=vec3(1.0)-exp(-result*exposure);
	
	if(use_gamma)
		result=pow(result,vec3(1.0/gamma));
	
	FragColor=vec4(result,1.0);
}