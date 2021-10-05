#pragma once
#include<iostream>
#include<format>
#include<vector>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"Camera.h"
#include"Shader.h"
#include"Model.h"

using namespace std;
using namespace glm;

extern Camera cam;
extern const int WIDTH;
extern const int HEIGHT;

enum class LightType {
	DirectionLight,
	PointLight,
	SpotLight
};

class Light {
public:
	Light(Shader* light_shader, const string& lightname, vec3 lightcolor, LightType ltype);
	virtual void set_lighting_to_obj_shader(Shader* obj_shader) = 0;
	virtual void draw() = 0;
	bool get_open_status();
	void set_open_status(bool open_status);
	Shader* shader;
	vec3 color;
	string name;
	const LightType type;
protected:
	bool opened;
};

class DirectionLight :public Light {
public:
	DirectionLight(Shader* light_shader, const string& lightname, vec3 lightcolor, vec3 lightdir) :Light(light_shader,lightname, lightcolor,LightType::DirectionLight), direction(lightdir) {};
	void set_lighting_to_obj_shader(Shader* obj_shader);
	void draw();
	vec3 direction;
};

class PointLight :public Light {
public:
	PointLight(Model* lightmodel, const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs) :Light(lightmodel->shader,lightname, lightcolor,LightType::PointLight),
		light_model(lightmodel), position(lightpos), origin_position(lightpos), _attenuation_coefs(attenuation_coefs) {
	};
	~PointLight();
	void set_lighting_to_obj_shader(Shader* obj_shader);
	void draw();

	Model* light_model;
	vec3 position;
	const vec3 origin_position;
	vec3 _attenuation_coefs;
};

class SpotLight :public Light {
public:
	SpotLight(Model *lightmodel, const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle) :Light(lightmodel->shader,lightname, lightcolor,LightType::SpotLight),
		light_model(lightmodel), position(lightpos), origin_position(lightpos), _attenuation_coefs(attenuation_coefs), spot_dir(spotdir), origin_spot_dir(spotdir), _inner_range_angle(inner_range_angle), _outer_range_angle(outer_range_angle) {
	};
	~SpotLight();
	void set_lighting_to_obj_shader(Shader* obj_shader);
	void draw();

	Model* light_model;
	vec3 position;
	const vec3 origin_position;
	vec3 spot_dir;
	const vec3 origin_spot_dir;
	vec3 _attenuation_coefs;
	float _inner_range_angle;
	float _outer_range_angle;
	
};