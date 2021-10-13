#version 330 core

struct Material{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;	//暂时用作法线贴图
	sampler2D texture_depth;		//深度贴图
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

in vec3 frag_world_pos;
in vec2 texcoord;
in mat3 TBN;

#define MAX_DIRECTION_LIGHTS_NUM 16
#define MAX_POINT_LIGHTS_NUM 16
#define MAX_SPOT_LIGHTS_NUM 16

uniform int parallax_option;
uniform float time;
uniform float parallax_scale;
uniform vec3 eye_pos;
uniform Material material;
uniform ivec3 lights_count;
uniform DirectionLight dir_lights[MAX_DIRECTION_LIGHTS_NUM];
uniform PointLight point_lights[MAX_POINT_LIGHTS_NUM];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS_NUM];

out vec4 FragColor;

vec3 cal_normal(vec2 texcoord_parallax){
	vec3 normal_tangent_space= vec3(texture(material.texture_specular0,texcoord_parallax))*2 -1.0;
	return TBN*normal_tangent_space;
}

vec2 cal_texcoord_parallax(vec3 view_vec,vec2 _texcoord){
	float depth=texture(material.texture_depth,_texcoord).r;
	vec3 view_vec_tangentspace=transpose(TBN)*view_vec;
	
	vec2 offset=-(view_vec_tangentspace.xy/view_vec_tangentspace.z)*(depth*parallax_scale);
	offset=vec2(offset.x,-offset.y);
	//offset=offset*2.0-1.0;
	return _texcoord+offset;
}

vec2 cal_texcoord_steep_parallax(vec3 view_vec,vec2 _texcoord){
	int layers_num=10;
	float layer_depth=1.0/layers_num;
	float cur_layer_depth=0.0;
	vec3 view_vec_tangentspace=transpose(TBN)*view_vec;
	vec3 p=-view_vec_tangentspace*parallax_scale;
	vec2 delta_texcoord=vec2(p.x,-p.y)/layers_num;

	vec2 cur_texcoord=_texcoord;
	float cur_sample_depth=texture(material.texture_depth,_texcoord).r;
	
	while(cur_sample_depth>cur_layer_depth){
		cur_texcoord=cur_texcoord+delta_texcoord;
		cur_sample_depth=texture(material.texture_depth,cur_texcoord).r;

		cur_layer_depth=cur_layer_depth+layer_depth;
	}
	return cur_texcoord;
}

vec2 cal_texcoord_parallax_occlusion(vec3 view_vec,vec2 _texcoord){
	int layers_num=10;
	float layer_depth=1.0/layers_num;
	float cur_layer_depth=0.0;
	vec3 view_vec_tangentspace=transpose(TBN)*view_vec;
	vec3 p=-view_vec_tangentspace*parallax_scale;
	vec2 delta_texcoord=vec2(p.x,-p.y)/layers_num;

	vec2 cur_texcoord=_texcoord;
	float cur_sample_depth=texture(material.texture_depth,_texcoord).r;
	
	while(cur_sample_depth>cur_layer_depth){
		cur_texcoord=cur_texcoord+delta_texcoord;
		cur_sample_depth=texture(material.texture_depth,cur_texcoord).r;
		cur_layer_depth=cur_layer_depth+layer_depth;
	}

	vec2 pre_texcoord=cur_texcoord-delta_texcoord;
	float pre_sample_depth=texture(material.texture_depth,pre_texcoord).r;
	float pre_layer_depth=cur_layer_depth-layer_depth;

	float delta_pre=pre_sample_depth-pre_layer_depth;
	float delta_cur=cur_layer_depth-cur_sample_depth;

	float weight=delta_cur/(delta_cur+delta_pre);
	vec2 lerp_texcoord=pre_texcoord*weight+cur_texcoord*(1.0-weight);

	return lerp_texcoord;
}

vec3 cal_direction_light(DirectionLight light){
	vec3 view_vec=normalize(eye_pos-frag_world_pos);
	vec2 texcoord_parallax=cal_texcoord_parallax(view_vec,texcoord);

	vec3 normal=cal_normal(texcoord_parallax);

	vec3 ambient=vec3(texture2D(material.texture_diffuse0,texcoord_parallax))*light.color*0.2f;
	vec3 direction=normalize(-light.direction);
	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(material.texture_diffuse0,texcoord_parallax))*light.color;
	diffuse=vec3(0.0);

	vec3 half_vec=normalize(direction+view_vec);
	vec3 specular;
	if(material.shininess>=1.0){
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),material.shininess);
		specular=specular_coef *vec3(texture2D(material.texture_diffuse0,texcoord_parallax))*light.color;

	}else{
		specular=vec3(0.0);
	}

	return ambient+diffuse+specular;
}

vec3 cal_point_light(PointLight light){
	vec3 view_vec=normalize(eye_pos-frag_world_pos);
	vec2 texcoord_parallax=vec2(0);
	if(parallax_option==0)
		texcoord_parallax=cal_texcoord_parallax(view_vec,texcoord);
	if(parallax_option==1)
		texcoord_parallax=cal_texcoord_steep_parallax(view_vec,texcoord);
	if(parallax_option==2)
		texcoord_parallax=cal_texcoord_parallax_occlusion(view_vec,texcoord);

	vec3 normal=cal_normal(texcoord_parallax);

	vec3 ambient=vec3(texture2D(material.texture_diffuse0,texcoord_parallax))*light.color*0.2f;

	vec3 direction=normalize(light.position-frag_world_pos);
	float distance_=length(light.position.xyz-frag_world_pos);
	float attenuation=1.0/(light.constant+light.linear*distance_+light.quadratic*distance_*distance_);

	float diffuse_coef=max(dot(direction,normalize(normal)),0);
	vec3 diffuse=diffuse_coef*vec3(texture2D(material.texture_diffuse0,texcoord_parallax))*light.color;

	vec3 specular;
	if(material.shininess>=1.0){
		
		vec3 half_vec=normalize(direction+view_vec);
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),material.shininess);
		specular=specular_coef *vec3(texture2D(material.texture_diffuse0,texcoord_parallax))*light.color;

		specular=specular_coef *vec3(1.0);
	}else{
		specular=vec3(0.0);
	}

	return (ambient+diffuse+specular)*attenuation;
}
vec3 cal_spot_light(SpotLight light){
	vec3 view_vec=normalize(eye_pos-frag_world_pos);
	vec2 texcoord_parallax=cal_texcoord_parallax(view_vec,texcoord);

	vec3 normal=cal_normal(texcoord_parallax);

	vec3 ambient=vec3(texture2D(material.texture_diffuse0,texcoord_parallax))*light.color*0.2f;

	vec3 direction=normalize(light.position.xyz-frag_world_pos);

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
	vec3 diffuse=diffuse_coef*vec3(texture2D(material.texture_diffuse0,texcoord_parallax))*light.color;

	
	vec3 half_vec=normalize(direction+view_vec);
	vec3 specular;
	if(material.shininess>=1.0){
		float specular_coef=pow(max(dot(half_vec,normalize(normal)),0),material.shininess);
		specular=specular_coef *vec3(texture2D(material.texture_specular0,texcoord_parallax))*light.color;
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

	vec3 emission=vec3(0.0);
	FragColor=vec4(result+emission,1.0);

}