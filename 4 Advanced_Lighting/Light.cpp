#include"Light.h"

//void Light::set_lighting_to_obj_shader(Shader& s) {}

void Light::open() {
	this->opened = true;
}

void Light::close() {
	this->opened = false;
}

void  DirectionLight::set_lighting_to_obj_shader(Shader* obj_shader) {
	obj_shader->use();
	obj_shader->set_uniform_3fv(format("{:s}.color", this->name), color);
	obj_shader->set_uniform_3fv(format("{:s}.direction", this->name), direction);
}

void DirectionLight::draw() {

}

PointLight::~PointLight() {
	delete light_model;
}

void  PointLight::set_lighting_to_obj_shader(Shader* obj_shader) {
	obj_shader->use();
	obj_shader->set_uniform_3fv(format("{:s}.color", this->name), color);
	obj_shader->set_uniform_3fv(format("{:s}.position", this->name), position);
	obj_shader->set_uniform_1f(format("{:s}.constant", this->name), _attenuation_coefs[0]);
	obj_shader->set_uniform_1f(format("{:s}.linear", this->name), _attenuation_coefs[1]);
	obj_shader->set_uniform_1f(format("{:s}.quadratic", this->name), _attenuation_coefs[2]);
}

void PointLight::draw() {

	float rotate_radians = (float)radians(30.0) * glfwGetTime();

	light_model->shader->use();
	light_model->shader->set_uniform_3fv("light_color", this->color);

	//lighting_model.rotate(lightingshader, rotate_radians, vec3(0, 1, 0));
	light_model->translate(this->origin_position);
	
	light_model->draw();
	
	mat4 light_model_matrix(1.0);
	//light_model_matrix = rotate(light_model_matrix, rotate_radians, vec3(0, 1, 0));
	light_model_matrix = translate(light_model_matrix, this->origin_position);	//在原位置进行model变换
	
	this->position = vec3(light_model_matrix * vec4(0.0, 0.0, 0.0, 1.0));
}

SpotLight::~SpotLight() {
	delete light_model;
}

void SpotLight::set_lighting_to_obj_shader(Shader* obj_shader) {
	obj_shader->use();
	obj_shader->set_uniform_3fv(format("{:s}.color", this->name), color);
	obj_shader->set_uniform_3fv(format("{:s}.position", this->name), position);
	obj_shader->set_uniform_3fv(format("{:s}.spot_dir", this->name), spot_dir);
	obj_shader->set_uniform_1f(format("{:s}.constant", this->name), _attenuation_coefs[0]);
	obj_shader->set_uniform_1f(format("{:s}.linear", this->name), _attenuation_coefs[1]);
	obj_shader->set_uniform_1f(format("{:s}.quadratic", this->name), _attenuation_coefs[2]);
	obj_shader->set_uniform_1f(format("{:s}.inner_range_angle", this->name), _inner_range_angle);
	obj_shader->set_uniform_1f(format("{:s}.outer_range_angle", this->name), _outer_range_angle);
}

void SpotLight::draw() {
	float rotate_radians = (float)radians(30.0) * glfwGetTime();

	light_model->shader->use();
	this->color.x = sin(radians(glfwGetTime() * 10)) / 2 + 0.5;
	this->color.y = sin(radians(glfwGetTime() * 10 + 120)) / 2 + 0.5;
	this->color.z = sin(radians(glfwGetTime() * 10 + 240)) / 2 + 0.5;
	light_model->shader->set_uniform_3fv("light_color", this->color);

	
	light_model->translate(this->origin_position);
	light_model->rotate(rotate_radians, vec3(0, 1, 0));

	light_model->draw();

	mat4 light_model_matrix(1.0);
	light_model_matrix = translate(light_model_matrix, this->origin_position);	//在原位置进行model变换
	light_model_matrix = rotate(light_model_matrix, rotate_radians, vec3(0, 1, 0));

	mat4 light_dir_rotate_matrix = mat4(1.0);	//注视方向向量只用变换旋转方向，不用变换位移
	light_dir_rotate_matrix = rotate(light_dir_rotate_matrix, rotate_radians, vec3(0, 1, 0));

	this->position = vec3(light_model_matrix * vec4(0.0, 0.0, 0.0, 1.0));
	this->spot_dir= vec3(light_dir_rotate_matrix * vec4(this->origin_spot_dir, 0.0));

}