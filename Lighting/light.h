#pragma once
#include<iostream>
#include<format>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"shader.h"

using namespace std;
using namespace glm;

enum class LightType {
	DirectionLight,
	PointLight,
	SpotLight
};

class Light {
public:
	Light(const string& lightname, vec3 lightcolor, LightType ltype) :name(lightname), color(lightcolor), opened(true),type(ltype) {};
	virtual void set_lighting_shader(Shader& s) = 0;
	void open();
	void close();
	string name;
	vec3 color;
	const LightType type;
protected:
	bool opened;
};

class DirectionLight :public Light {
public:
	DirectionLight(const string& lightname, vec3 lightcolor, vec3 lightdir) :Light(lightname, lightcolor,LightType::DirectionLight), direction(lightdir) {};
	void set_lighting_shader(Shader& s);
	vec3 direction;
};

class PointLight :public Light {
public:
	PointLight(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs) :Light(lightname, lightcolor,LightType::PointLight),
		position(lightpos), origin_postion(lightpos), _attenuation_coefs(attenuation_coefs) {
	};
	void set_lighting_shader(Shader& s);
	vec3 position;
	const vec3 origin_postion;
	vec3 _attenuation_coefs;
};

class SpotLight :public Light {
public:
	SpotLight(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle) :Light(lightname, lightcolor,LightType::SpotLight),
		position(lightpos), origin_postion(lightpos), _attenuation_coefs(attenuation_coefs), spot_dir(spotdir), origin_spot_dir(spotdir), _inner_range_angle(inner_range_angle), _outer_range_angle(outer_range_angle) {
	};
	void set_lighting_shader(Shader& s);
	vec3 position;
	const vec3 origin_postion;
	vec3 spot_dir;
	const vec3 origin_spot_dir;
	vec3 _attenuation_coefs;
	float _inner_range_angle;
	float _outer_range_angle;
	
};

class LightManager {
public:
	static void create_direction_light(const string& lightname, vec3 lightcolor, vec3 lightdir);
	static void create_point_light(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs);
	static void create_spot_light(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle);
	static const int get_lights_num();
	static Light* lights[16];
private:
	LightManager() {};
	static int nums;
};
