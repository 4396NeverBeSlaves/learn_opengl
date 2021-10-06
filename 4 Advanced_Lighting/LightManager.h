#pragma once
#include"Light.h"

class LightManager {
public:
	static void create_direction_light(Shader* shader, vec3 lightcolor, vec3 lightdir);
	static void create_point_light(Model* lightmodel, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs);
	static void create_spot_light(Model* lightmodel, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle);
	static void draw();
	static void update_lighting_info_in_obj_shader(Shader* obj_shader);
	static const int get_all_lights_num(LightType ltype);
	static const int get_opened_lights_num(LightType ltype);
	static void destory_all_lights();
	static vector<Light*> lights;
	static vector<int> lights_idx_in_shader;
private:
	LightManager() {};
	static void update_lights_idx_in_shader();
};