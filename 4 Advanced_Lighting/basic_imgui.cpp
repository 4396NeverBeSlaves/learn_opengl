#include<iostream>
#include<format>
#include<vector>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"Camera.h"
#include"Shader.h"
#include"Light.h"
#include"LightManager.h"
#include"Model.h"
#include"ModelManager.h"


vector<Texture> textures;

using namespace std;
using namespace glm;

const int WIDTH = 900;
const int HEIGHT = 900;
vec3 cam_pos(0, 2, 8);
vec3 cam_dir(0, 0, -1);
vec3 cam_up(0, 1, 0);
Camera cam(cam_pos, cam_dir, cam_up);

float current_time = 0.0;
float old_time;
float delta_time;
float get_delta_time() {
	old_time = current_time;
	current_time = glfwGetTime();
	return current_time - old_time;
}

void framebuf_size_callback(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
	int cw, ch;
	glfwGetFramebufferSize(w, &cw, &ch);
	cout << "frame buf w:" << cw << " h:" << ch << endl;

	cout << "resizing window: w:" << width << " h: " << height << endl << endl;
}

float tranparency = 0.5;
void key_callback(GLFWwindow* w, int key, int scannode, int action, int mods) {
	float cam_speed = 20 * delta_time;
	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(w, true);
	}
	if (glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS) {
		if (tranparency < 1.0)
			tranparency += 0.01;
	}
	if (glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (tranparency > 0.1)
			tranparency -= 0.01;
	}
	if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
		cam.move(Move_direction::FORWARD, delta_time);
	}
	if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
		cam.move(Move_direction::BACK, delta_time);
	}
	if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
		cam.move(Move_direction::LEFT, delta_time);
	}
	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
		cam.move(Move_direction::RIGHT, delta_time);
	}
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	cam.look(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//cout << "x:" << xoffset << " y:" << yoffset << endl;
	cam.zoom(xoffset, yoffset);
}

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main() {
	init();
	GLFWwindow* w = glfwCreateWindow(WIDTH, HEIGHT, "Lighting.", NULL, NULL);
	if (!w) {
		cout << "glfwCreateWindow failed!" << endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(w);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "failed to init glad!" << endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(w, framebuf_size_callback);
	glfwSetKeyCallback(w, key_callback);
	glfwSetScrollCallback(w, scroll_callback);
	glfwSetCursorPosCallback(w, cursor_callback);

	float time1 = glfwGetTime();

	Shader* objshader = new Shader("basic_imgui_obj_shader.vert", "basic_imgui_obj_shader.frag");
	Shader* lightingshader = new Shader("basic_imgui_light_shader.vert", "basic_imgui_light_shader.frag");

	Model* plane = new Model(R"(C:\Users\X\Desktop\plane.obj)", objshader);
	Model* box1 = new Model(R"(C:\Users\X\Desktop\box_marble.obj)", objshader);
	Model* box2 = new Model(R"(C:\Users\X\Desktop\box_marble.obj)", objshader);
	Model* box3 = new Model(R"(C:\Users\X\Desktop\box_marble.obj)", objshader);
	ModelManger::add_model(plane);
	ModelManger::add_model(box1);
	ModelManger::add_model(box2);
	ModelManger::add_model(box3);

	vec3 light_color(1.0, 1.0, 1.0);
	vec3 light_coef(1.0, 0.007, 0.00028);
	Model* light_box = new Model(R"(C:\Users\X\Desktop\box.obj)", lightingshader);

	LightManager::create_direction_light(lightingshader, 0.3f*vec3(1.0, 1.0, 1.0), vec3(0.0, -0.7, -0.7));
	LightManager::create_point_light(light_box, vec3(1.0, 1.0, 1.0), vec3(-5, 11, -6), light_coef);
	LightManager::create_point_light(light_box, vec3(0.0, 1.0, 0.0), vec3(6.6, 13.3, 2.5), light_coef);
	LightManager::create_point_light(light_box, vec3(0.0, 0.0, 1.0), vec3(0.0, 3.0, -10.0), light_coef);
	LightManager::create_point_light(light_box, vec3(1.0, 0.0, 0.0), vec3(-3, 2.6, 6.5), light_coef);
	LightManager::create_spot_light(light_box, vec3(1.0, 1.0, 1.0), vec3(2.0, 0.6, 3.2), light_coef, vec3(0.0, 0.0, -1.0), 15.0, 17.0);

	float one_second = 0;
	int frame = 0;

	while (!glfwWindowShouldClose(w)) {
		delta_time = get_delta_time();
		one_second += delta_time;
		frame++;
		if (one_second >= 1.0) {
			string title = format("Shadow Mapping. [{:6.1f}FPS, {:5.2f}ms] [FOV: {:4.1f}] [Yaw:{:7.1f}, Pitch:{:5.1f}] [Position:{:5.1f} {:5.1f} {:5.1f}, Direction:{:4.1f} {:4.1f} {:4.1f}]",
				frame / one_second, one_second / frame * 1000, cam.fov, cam.yaw_, cam.pitch_, cam.cam_pos.x, cam.cam_pos.y, cam.cam_pos.z, cam.cam_dir.x, cam.cam_dir.y, cam.cam_dir.z);
			glfwSetWindowTitle(w, title.c_str());
			one_second = 0.0;
			frame = 0;
		}

		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LightManager::lights[1]->set_open_status(false);

		LightManager::draw();
		LightManager::update_lighting_info_in_obj_shader(objshader);

		objshader->use();
		objshader->set_matrix("view", cam.get_view_matrix());
		objshader->set_matrix("projection", perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f));
		objshader->set_uniform_3fv("eye_pos", cam.cam_pos);

		ModelManger::models[2]->translate(vec3(2, 2, 2));
		ModelManger::models[3]->translate(vec3(-1, 0, 3));
		ModelManger::draw();


		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	objshader->delete_program();
	lightingshader->delete_program();
	delete objshader;
	delete lightingshader;
	LightManager::destory_all_lights();
	ModelManger::destroy_all_models();
	glfwTerminate();
	return 0;
}
