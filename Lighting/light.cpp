#include"light.h"

void Light::set_light(Shader& s) {}

void  DirectionLight::set_light(Shader& s) {
	s.set_uniform_3fv(format("{:s}.color", this->name), color);
	s.set_uniform_3fv(format("{:s}.direction", this->name), direction);
}

void  PointLight::set_light(Shader& s) {
	s.set_uniform_3fv(format("{:s}.color", this->name), color);
	s.set_uniform_3fv(format("{:s}.position", this->name), position);
	s.set_uniform_1f(format("{:s}.constant", this->name), _attenuation_coefs[0]);
	s.set_uniform_1f(format("{:s}.linear", this->name), _attenuation_coefs[1]);
	s.set_uniform_1f(format("{:s}.quadratic", this->name), _attenuation_coefs[2]);
}

void SpotLight::set_light(Shader& s) {
	s.set_uniform_3fv(format("{:s}.color", this->name), color);
	s.set_uniform_3fv(format("{:s}.position", this->name), position);
	s.set_uniform_3fv(format("{:s}.spot_dir", this->name), spot_dir);
	s.set_uniform_1f(format("{:s}.constant", this->name), _attenuation_coefs[0]);
	s.set_uniform_1f(format("{:s}.linear", this->name), _attenuation_coefs[1]);
	s.set_uniform_1f(format("{:s}.quadratic", this->name), _attenuation_coefs[2]);
	s.set_uniform_1f(format("{:s}.inner_range_angle", this->name), _inner_range_angle);
	s.set_uniform_1f(format("{:s}.outer_range_angle", this->name), _outer_range_angle);
}