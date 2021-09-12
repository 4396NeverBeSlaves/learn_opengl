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
	Light(const string& lightname, vec3 lightcolor, LightType ltype) :name(lightname), color(lightcolor), opened(true),type(ltype) {};
	virtual void set_lighting_to_obj_shader(Shader& s) = 0;
	virtual void draw(Shader& lightingshader, Model& lighting_model) = 0;
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
	void set_lighting_to_obj_shader(Shader& s);
	void draw(Shader& lightingshader, Model& lighting_model);
	vec3 direction;
};

class PointLight :public Light {
public:
	PointLight(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs) :Light(lightname, lightcolor,LightType::PointLight),
		position(lightpos), origin_position(lightpos), _attenuation_coefs(attenuation_coefs) {
	};
	void set_lighting_to_obj_shader(Shader& s);
	void draw(Shader& lightingshader, Model& lighting_model);

	vec3 position;
	const vec3 origin_position;
	vec3 _attenuation_coefs;
};

class SpotLight :public Light {
public:
	SpotLight(const string& lightname, vec3 lightcolor, vec3 lightpos, vec3 attenuation_coefs, vec3 spotdir, float inner_range_angle, float outer_range_angle) :Light(lightname, lightcolor,LightType::SpotLight),
		position(lightpos), origin_position(lightpos), _attenuation_coefs(attenuation_coefs), spot_dir(spotdir), origin_spot_dir(spotdir), _inner_range_angle(inner_range_angle), _outer_range_angle(outer_range_angle) {
	};
	void set_lighting_to_obj_shader(Shader& s);
	void draw(Shader& lightingshader, Model& lighting_model);

	vec3 position;
	const vec3 origin_position;
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
	static void destory_all_lights();
	static vector<Light*> lights;
private:
	LightManager() {};
};