#include"Light.h"

//void Light::set_lighting_to_obj_shader(Shader& s) {}

void Light::open() {
	this->opened = true;
}

void Light::close() {
	this->opened = false;
}

void  DirectionLight::set_lighting_to_obj_shader(Shader& s) {
	s.use();
	s.set_uniform_3fv(format("{:s}.color", this->name), color);
	s.set_uniform_3fv(format("{:s}.direction", this->name), direction);
}

void DirectionLight::draw(Shader& lightingshader,Model& lighting_model) {

}

void  PointLight::set_lighting_to_obj_shader(Shader& s) {
	s.use();
	s.set_uniform_3fv(format("{:s}.color", this->name), color);
	s.set_uniform_3fv(format("{:s}.position", this->name), position);
	s.set_uniform_1f(format("{:s}.constant", this->name), _attenuation_coefs[0]);
	s.set_uniform_1f(format("{:s}.linear", this->name), _attenuation_coefs[1]);
	s.set_uniform_1f(format("{:s}.quadratic", this->name), _attenuation_coefs[2]);
}

void PointLight::draw(Shader& lightingshader, Model& lighting_model) {

	float rotate_radians = (float)radians(30.0) * glfwGetTime();

	lightingshader.use();
	lightingshader.set_uniform_3fv("light_color", this->color);

	//lighting_model.rotate(lightingshader, rotate_radians, vec3(0, 1, 0));
	lighting_model.translate(lightingshader, this->origin_position);
	
	lighting_model.draw(lightingshader);
	
	mat4 light_model_matrix(1.0);
	//light_model_matrix = rotate(light_model_matrix, rotate_radians, vec3(0, 1, 0));
	light_model_matrix = translate(light_model_matrix, this->origin_position);	//在原位置进行model变换
	
	this->position = vec3(light_model_matrix * vec4(0.0, 0.0, 0.0, 1.0));
}

void SpotLight::set_lighting_to_obj_shader(Shader& s) {
	s.use();
	s.set_uniform_3fv(format("{:s}.color", this->name), color);
	s.set_uniform_3fv(format("{:s}.position", this->name), position);
	s.set_uniform_3fv(format("{:s}.spot_dir", this->name), spot_dir);
	s.set_uniform_1f(format("{:s}.constant", this->name), _attenuation_coefs[0]);
	s.set_uniform_1f(format("{:s}.linear", this->name), _attenuation_coefs[1]);
	s.set_uniform_1f(format("{:s}.quadratic", this->name), _attenuation_coefs[2]);
	s.set_uniform_1f(format("{:s}.inner_range_angle", this->name), _inner_range_angle);
	s.set_uniform_1f(format("{:s}.outer_range_angle", this->name), _outer_range_angle);
}

void SpotLight::draw(Shader& lightingshader, Model& lighting_model) {
	float rotate_radians = (float)radians(30.0) * glfwGetTime();

	lightingshader.use();
	this->color.x = sin(radians(glfwGetTime() * 10)) / 2 + 0.5;
	this->color.y = sin(radians(glfwGetTime() * 10 + 120)) / 2 + 0.5;
	this->color.z = sin(radians(glfwGetTime() * 10 + 240)) / 2 + 0.5;
	lightingshader.set_uniform_3fv("light_color", this->color);

	
	lighting_model.translate(lightingshader, this->origin_position);
	lighting_model.rotate(lightingshader, rotate_radians, vec3(0, 1, 0));

	lighting_model.draw(lightingshader);

	mat4 light_model_matrix(1.0);
	light_model_matrix = translate(light_model_matrix, this->origin_position);	//在原位置进行model变换
	light_model_matrix = rotate(light_model_matrix, rotate_radians, vec3(0, 1, 0));

	mat4 light_dir_rotate_matrix = mat4(1.0);	//注视方向向量只用变换旋转方向，不用变换位移
	light_dir_rotate_matrix = rotate(light_dir_rotate_matrix, rotate_radians, vec3(0, 1, 0));

	this->position = vec3(light_model_matrix * vec4(0.0, 0.0, 0.0, 1.0));
	this->spot_dir= vec3(light_dir_rotate_matrix * vec4(this->origin_spot_dir, 0.0));

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
	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}
}
vector<Light*> LightManager::lights;