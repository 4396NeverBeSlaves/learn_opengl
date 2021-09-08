#pragma once
#include<iostream>
#include<format>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"shader.h"

using namespace std;
using namespace glm;

class Light {
public:
	Light(const string& lightname, vec3 lightcolor) :name(lightname), color(lightcolor) {};
	virtual void set_light(Shader& s) = 0;
	string name;
	vec3 color;
};

class DirectionLight :public Light {
public:
	DirectionLight(const string& lightname, vec3 lightcolor, vec3 lightdir) :Light(lightname, lightcolor), direction(lightdir) {};
	void set_light(Shader& s);
	vec3 direction;
};

class PointLight :public Light {
public:
	PointLight(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs) :Light(lightname, lightcolor),
		position(lightpos), origin_postion(lightpos), _attenuation_coefs(attenuation_coefs) {
	};
	void set_light(Shader& s);
	vec3 position;
	const vec3 origin_postion;
private:
	vec3 _attenuation_coefs;
};

class SpotLight :public Light {
public:
	SpotLight(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle) :Light(lightname, lightcolor),
		position(lightpos), origin_postion(lightpos), _attenuation_coefs(attenuation_coefs), spot_dir(spotdir), origin_spot_dir(spotdir), _inner_range_angle(inner_range_angle), _outer_range_angle(outer_range_angle) {
	};
	void set_light(Shader& s);
	vec3 position;
	const vec3 origin_postion;
	vec3 spot_dir;
	const vec3 origin_spot_dir;
private:
	vec3 _attenuation_coefs;
	float _inner_range_angle;
	float _outer_range_angle;
};