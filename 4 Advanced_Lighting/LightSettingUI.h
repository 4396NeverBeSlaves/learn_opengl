#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include"Light.h"
#include"LightManager.h"

extern	vec3 background_color;
extern	Shader* lightingshader;
extern	Model* light_box;

namespace LightSettingUI {
	void display();
}