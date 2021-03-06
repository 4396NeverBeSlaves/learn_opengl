#include"light.h"

//void Light::set_lighting_shader(Shader& s) {}

void Light::open() {
	this->opened = true;
}

void Light::close() {
	this->opened = false;
}

void  DirectionLight::set_lighting_shader(Shader& s) {
	s.set_uniform_3fv(format("{:s}.color", this->name), color);
	s.set_uniform_3fv(format("{:s}.direction", this->name), direction);
}

void  PointLight::set_lighting_shader(Shader& s) {
	s.set_uniform_3fv(format("{:s}.color", this->name), color);
	s.set_uniform_3fv(format("{:s}.position", this->name), position);
	s.set_uniform_1f(format("{:s}.constant", this->name), _attenuation_coefs[0]);
	s.set_uniform_1f(format("{:s}.linear", this->name), _attenuation_coefs[1]);
	s.set_uniform_1f(format("{:s}.quadratic", this->name), _attenuation_coefs[2]);
}

void SpotLight::set_lighting_shader(Shader& s) {
	s.set_uniform_3fv(format("{:s}.color", this->name), color);
	s.set_uniform_3fv(format("{:s}.position", this->name), position);
	s.set_uniform_3fv(format("{:s}.spot_dir", this->name), spot_dir);
	s.set_uniform_1f(format("{:s}.constant", this->name), _attenuation_coefs[0]);
	s.set_uniform_1f(format("{:s}.linear", this->name), _attenuation_coefs[1]);
	s.set_uniform_1f(format("{:s}.quadratic", this->name), _attenuation_coefs[2]);
	s.set_uniform_1f(format("{:s}.inner_range_angle", this->name), _inner_range_angle);
	s.set_uniform_1f(format("{:s}.outer_range_angle", this->name), _outer_range_angle);
}

void LightManager::create_direction_light(const string& lightname, vec3 lightcolor, vec3 lightdir) {
	lights.push_back(new DirectionLight(lightname, lightcolor, lightdir));
}

void LightManager::create_point_light(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs) {
	lights.push_back(new PointLight(lightname, lightcolor, lightpos,attenuation_coefs));
}

void LightManager::create_spot_light(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle) {
	lights.push_back(new SpotLight(lightname, lightcolor,  lightpos, attenuation_coefs,  spotdir,  inner_range_angle,  outer_range_angle));
}

const int LightManager::get_lights_num() {
	return lights.size();
}

void LightManager::destory_all_lights() {
	for (int i = 0; i++; i < lights.size()) {
		delete lights[i];
	}
}
vector<Light*> LightManager::lights;