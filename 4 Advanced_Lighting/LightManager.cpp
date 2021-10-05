#include"LightManager.h"

vector<Light*> LightManager::lights;

void LightManager::create_direction_light(Shader* shader, vec3 lightcolor, vec3 lightdir) {
	string lightname = format("dir_lights[{:d}]", LightManager::get_all_lights_num(LightType::DirectionLight));
	lights.push_back(new DirectionLight(shader,lightname, lightcolor, lightdir));
}

void LightManager::create_point_light(Model* lightmodel, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs) {
	string lightname = format("point_lights[{:d}]", LightManager::get_all_lights_num(LightType::PointLight));
	lights.push_back(new PointLight(lightmodel, lightname, lightcolor, lightpos, attenuation_coefs));
}

void LightManager::create_spot_light(Model* lightmodel, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle) {
	string lightname = format("spot_lights[{:d}]", LightManager::get_all_lights_num(LightType::SpotLight));
	lights.push_back(new SpotLight(lightmodel, lightname, lightcolor, lightpos, attenuation_coefs, spotdir, inner_range_angle, outer_range_angle));
}

void LightManager::update_lights_name() {
	int dir_num = 0, point_num = 0, spot_num = 0;

	for (size_t i = 0; i < lights.size(); i++) {
		if (lights[i]->get_open_status() == true) {
			switch (lights[i]->type) {
			case LightType::DirectionLight:
				lights[i]->name = format("dir_lights[{:d}]", dir_num++);
				break;
			case LightType::PointLight:
				lights[i]->name = format("point_lights[{:d}]", point_num++);
				break;
			case LightType::SpotLight:
				lights[i]->name = format("spot_lights[{:d}]", spot_num++);
				break;
			default:
				break;
			}
		}
	}
	
}

void LightManager::draw() {
	LightManager::update_lights_name();

	for (int i = 0; i < lights.size(); i++) {
		LightManager::lights[i]->draw();
	}
}

void LightManager::update_lighting_info_in_obj_shader(Shader* obj_shader) {
	obj_shader->use();
	obj_shader->set_uniform_3iv("lights_count",
		ivec3(LightManager::get_opened_lights_num(LightType::DirectionLight),
			LightManager::get_opened_lights_num(LightType::PointLight),
			LightManager::get_opened_lights_num(LightType::SpotLight)));

	for (int i = 0; i < lights.size(); i++) {
		LightManager::lights[i]->set_lighting_to_obj_shader(obj_shader);
	}
}

const int LightManager::get_all_lights_num(LightType ltype) {
	int count = 0;
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i]->type == ltype) {
			count++;
		}
	}
	return count;
}

const int LightManager::get_opened_lights_num(LightType ltype) {
	int count = 0;
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i]->type == ltype && lights[i]->get_open_status()==true) {
			count++;
		}
	}
	return count;
}

void LightManager::destory_all_lights() {
	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}
}