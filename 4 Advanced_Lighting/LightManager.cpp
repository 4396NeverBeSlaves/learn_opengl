#include"LightManager.h"

vector<Light*> LightManager::lights;
vector<int> LightManager::lights_idx_in_shader;

void LightManager::create_direction_light(Shader* shader, vec3 lightcolor, vec3 lightdir) {
	int idx = LightManager::get_all_lights_num(LightType::DirectionLight);
	lights_idx_in_shader.push_back(idx);
	string lightname = format("DirectionLight {:d}", idx+1);
	lights.push_back(new DirectionLight(shader,lightname, lightcolor, lightdir));
}

void LightManager::create_point_light(Model* lightmodel, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs) {
	int idx = LightManager::get_all_lights_num(LightType::PointLight);
	lights_idx_in_shader.push_back(idx);
	string lightname = format("PointLight {:d}",idx+1);
	lights.push_back(new PointLight(lightmodel, lightname, lightcolor, lightpos, attenuation_coefs));
}

void LightManager::create_spot_light(Model* lightmodel, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle) {
	int idx = LightManager::get_all_lights_num(LightType::SpotLight);
	lights_idx_in_shader.push_back(idx);
	string lightname = format("SpotLight {:d}", idx+1);
	lights.push_back(new SpotLight(lightmodel, lightname, lightcolor, lightpos, attenuation_coefs, spotdir, inner_range_angle, outer_range_angle));
}

void LightManager::update_lights_idx_in_shader() {
	int dir_num = 0, point_num = 0, spot_num = 0;

	for (size_t i = 0; i < lights.size(); i++) {
		if (lights[i]->get_open_status() == true) {
			switch (lights[i]->type) {
			case LightType::DirectionLight:
				lights_idx_in_shader[i]=dir_num++;
				break;
			case LightType::PointLight:
				lights_idx_in_shader[i]=point_num++;
				break;
			case LightType::SpotLight:
				lights_idx_in_shader[i]=spot_num++;
				break;
			default:
				break;
			}
		}
		else {
			lights_idx_in_shader[i]=-1;
		}
	}
	
}

void LightManager::draw() {
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

	LightManager::update_lights_idx_in_shader();
	for (int i = 0; i < lights.size(); i++) {
		LightManager::lights[i]->set_lighting_to_obj_shader(obj_shader,lights_idx_in_shader[i]);
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