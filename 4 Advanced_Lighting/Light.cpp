#include"Light.h"

//void Light::set_lighting_to_obj_shader(Shader& s) {}

Light::Light(Shader* light_shader, const string& lightname, vec3 lightcolor, LightType ltype) :shader(light_shader), name(lightname), color(lightcolor), opened(true), type(ltype) {
	
}

bool Light::get_open_status() {
	return this->opened;
}

void Light::set_open_status(bool open_status) {
	this->opened = open_status;
}

void  DirectionLight::set_lighting_to_obj_shader(Shader* obj_shader, int idx) {
	if (this->opened == false)
		return;

	obj_shader->use();
	obj_shader->set_uniform_3fv(format("dir_lights[{:d}].color", idx), color);
	obj_shader->set_uniform_3fv(format("dir_lights[{:d}].direction", idx), direction);
}

void DirectionLight::draw() {

}

PointLight::~PointLight() {
	delete light_model;
}

void  PointLight::set_lighting_to_obj_shader(Shader* obj_shader, int idx) {
	if (this->opened == false)
		return;

	obj_shader->use();
	obj_shader->set_uniform_3fv(format("point_lights[{:d}].color", idx), color);
	obj_shader->set_uniform_3fv(format("point_lights[{:d}].position", idx), position);
	obj_shader->set_uniform_1f(format("point_lights[{:d}].constant", idx), _attenuation_coefs[0]);
	obj_shader->set_uniform_1f(format("point_lights[{:d}].linear", idx), _attenuation_coefs[1]);
	obj_shader->set_uniform_1f(format("point_lights[{:d}].quadratic", idx), _attenuation_coefs[2]);
}

void PointLight::draw() {
	if (this->opened == false)
		return;

	float rotate_radians = (float)radians(30.0) * glfwGetTime();

	light_model->shader->use();
	light_model->shader->set_matrix("view", cam.get_view_matrix());
	light_model->shader->set_matrix("projection", perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f));
	light_model->shader->set_uniform_3fv("light_color", this->color);

	light_model->translate(this->origin_position);
	light_model->scale(vec3(0.1,0.1,0.1));
	light_model->draw();
	

	mat4 light_model_matrix(1.0);
	light_model_matrix = translate(light_model_matrix, this->origin_position);	//在原位置进行model变换

	this->position = vec3(light_model_matrix * vec4(0.0, 0.0, 0.0, 1.0));
}

SpotLight::~SpotLight() {
	delete light_model;
}

void SpotLight::set_lighting_to_obj_shader(Shader* obj_shader, int idx) {
	if (this->opened == false)
		return;

	obj_shader->use();
	obj_shader->set_uniform_3fv(format("spot_lights[{:d}].color", idx), color);
	obj_shader->set_uniform_3fv(format("spot_lights[{:d}].position", idx), position);
	obj_shader->set_uniform_3fv(format("spot_lights[{:d}].spot_dir", idx), spot_dir);
	obj_shader->set_uniform_1f(format("spot_lights[{:d}].constant", idx), _attenuation_coefs[0]);
	obj_shader->set_uniform_1f(format("spot_lights[{:d}].linear", idx), _attenuation_coefs[1]);
	obj_shader->set_uniform_1f(format("spot_lights[{:d}].quadratic", idx), _attenuation_coefs[2]);
	obj_shader->set_uniform_1f(format("spot_lights[{:d}].inner_range_angle", idx), _inner_range_angle);
	obj_shader->set_uniform_1f(format("spot_lights[{:d}].outer_range_angle", idx), _outer_range_angle);
}

void SpotLight::draw() {
	if (this->opened == false)
		return;

	float rotate_radians = (float)radians(30.0) * glfwGetTime();

	light_model->shader->use();
	this->color.x = sin(radians(glfwGetTime() * 10)) / 2 + 0.5;
	this->color.y = sin(radians(glfwGetTime() * 10 + 120)) / 2 + 0.5;
	this->color.z = sin(radians(glfwGetTime() * 10 + 240)) / 2 + 0.5;
	light_model->shader->set_uniform_3fv("light_color", this->color);
	light_model->shader->set_matrix("view", cam.get_view_matrix());
	light_model->shader->set_matrix("projection", perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f));

	
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