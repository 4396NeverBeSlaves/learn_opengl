#include"LightManager.h"

vector<Light*> LightManager::lights;

void LightManager::create_direction_light(Shader* shader,const string& lightname, vec3 lightcolor, vec3 lightdir) {
	lights.push_back(new DirectionLight(shader,lightname, lightcolor, lightdir));
}

void LightManager::create_point_light(Model* lightmodel, const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs) {
	lights.push_back(new PointLight(lightmodel, lightname, lightcolor, lightpos, attenuation_coefs));
}

void LightManager::create_spot_light(Model* lightmodel, const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle) {
	lights.push_back(new SpotLight(lightmodel, lightname, lightcolor, lightpos, attenuation_coefs, spotdir, inner_range_angle, outer_range_angle));
}

void LightManager::draw() {
	for (int i = 0; i < LightManager::get_lights_num(); i++) {
		LightManager::lights[i]->draw();
	}
}

void LightManager::update_lighting_info_in_obj_shader(Shader* obj_shader) {
	for (int i = 0; i < LightManager::get_lights_num(); i++) {
		LightManager::lights[i]->set_lighting_to_obj_shader(obj_shader);
	}
}

const int LightManager::get_lights_num() {
	return lights.size();
}

void LightManager::destory_all_lights() {
	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}
}
