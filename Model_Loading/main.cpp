#include<iostream>
#include<format>
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
#include"Model.h"

vector<Texture> textures;

using namespace std;
using namespace glm;

const int WIDTH = 900;
const int HEIGHT = 900;
vec3 cam_pos(0, 10, 18);
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
	Model nanosuit(R"(C:\Users\X\Desktop\nanosuit\nanosuit.obj)");
	cout << "loading time: " << glfwGetTime() -time1 << endl;
	Model wood_box(R"(C:\Users\X\AppData\Roaming\pico-8\appdata\toolbox\box_yellow.obj)");

	Model light_box(R"(C:\Users\X\Desktop\box.obj)");

	Shader objshader("nanosuit_shader.vert", "nanosuit_shader.frag");
	Shader lightingshader("light_shader.vert", "light_shader.frag");

	vec3 light_color(1.0, 1.0, 1.0);
	vec3 light_coef(1.0, 0.007, 0.00028);

	LightManager::create_direction_light("dir_light", 0.3f*vec3(1.0, 1.0, 1.0), vec3(0.0, -0.7, -0.7));
	LightManager::create_point_light("point_lights[0]", vec3(1.0, 1.0, 1.0), vec3(-5, 11, -6), light_coef);
	LightManager::create_point_light("point_lights[1]", vec3(0.0, 1.0, 0.0), vec3(6.6, 13.3, 2.5), light_coef);
	LightManager::create_point_light("point_lights[2]", vec3(0.0, 0.0, 1.0), vec3(0.0, 3.0, -10.0), light_coef);
	LightManager::create_point_light("point_lights[3]", vec3(1.0, 0.0, 0.0), vec3(-8, 7.6, -3.8), light_coef);
	LightManager::create_spot_light("spot_light", vec3(1.0, 1.0, 1.0), vec3(0.0, 10.6, 10.0), light_coef, vec3(0.0, 0.0, -1.0), 15.0, 17.0);

	float one_second = 0;
	int frame = 0;

	while (!glfwWindowShouldClose(w)) {
		delta_time = get_delta_time();
		one_second += delta_time;
		frame++;
		if (one_second >= 1.0) {
			string title = format("Model loading. [{:6.1f}FPS, {:5.2f}ms] [FOV: {:4.1f}] [Yaw:{:7.1f}, Pitch:{:5.1f}] [Position:{:5.1f} {:5.1f} {:5.1f}, Direction:{:4.1f} {:4.1f} {:4.1f}]",
				frame / one_second, one_second / frame * 1000, cam.fov, cam.yaw_, cam.pitch_, cam.cam_pos.x, cam.cam_pos.y, cam.cam_pos.z, cam.cam_dir.x, cam.cam_dir.y, cam.cam_dir.z);
			glfwSetWindowTitle(w, title.c_str());
			one_second = 0.0;
			frame = 0;
		}

		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < LightManager::get_lights_num(); i++) {
			LightManager::lights[i]->draw(lightingshader,light_box);
			LightManager::lights[i]->set_lighting_to_obj_shader(objshader);
		}

		objshader.use();
		objshader.set_uniform_1f("time", current_time);

		wood_box.translate(objshader, vec3(1.4, 10.4, 3.8));
		//wood_box.rotate(objshader, (float)radians(20.0) * current_time, vec3(0, 1, 0));
		wood_box.draw(objshader);

		//nanosuit.translate(objshader, vec3(3, 0, 0));
		nanosuit.rotate(objshader, (float)radians(20.0) * current_time, vec3(0, 1, 0));
		nanosuit.draw(objshader);

		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	objshader.delete_program();
	//lightshader.delete_program();
	LightManager::destory_all_lights();
	glfwTerminate();
	return 0;
}