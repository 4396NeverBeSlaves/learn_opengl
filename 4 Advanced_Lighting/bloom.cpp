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
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include"Camera.h"
#include"Shader.h"
#include"Light.h"
#include"LightManager.h"
#include"Model.h"
#include"ModelManager.h"
#include"LightSettingUI.h"

//namespace _bloom__{
using namespace std;
using namespace glm;

vector<Texture> textures;
vec3 background_color = vec3(0.2, 0.2, 0.2);
Shader* lightingshader;
Model* light_box;

const int WIDTH = 900;
const int HEIGHT = 900;
vec3 cam_pos(-1, 4.5, 1);
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

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	cam.look(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//cout << "x:" << xoffset << " y:" << yoffset << endl;
	cam.zoom(xoffset, yoffset);
}

float tranparency = 0.5;
void key_callback(GLFWwindow* w, int key, int scannode, int action, int mods) {
	float cam_speed = 20 * delta_time;
	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(w, true);
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
	if (glfwGetKey(w, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
		int cursor_status = glfwGetInputMode(w, GLFW_CURSOR);
		if (cursor_status == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPosCallback(w, NULL);
		}
		if (cursor_status == GLFW_CURSOR_NORMAL) {
			glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(w, cursor_callback);
		}
	}
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

	glfwSwapInterval(1);
	glViewport(0, 0, WIDTH, HEIGHT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(w, framebuf_size_callback);
	glfwSetKeyCallback(w, key_callback);
	glfwSetScrollCallback(w, scroll_callback);
	glfwSetCursorPosCallback(w, cursor_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(w, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	float time1 = glfwGetTime();

	lightingshader = new Shader(R"(..\4 Advanced_Lighting\bloom_light_shader.vert)", R"(..\4 Advanced_Lighting\bloom_light_shader.frag)");
	light_box = new Model(R"(..\Assets\box.obj)", lightingshader);

	Shader* objshader = new Shader(R"(..\4 Advanced_Lighting\bloom_obj_shader.vert)", R"(..\4 Advanced_Lighting\bloom_obj_shader.frag)");
	Shader* screen_shader = new Shader(R"(..\4 Advanced_Lighting\bloom_screen_shader.vert)", R"(..\4 Advanced_Lighting\bloom_screen_shader.frag)");

	Model* wood_box = new Model(R"(..\Assets\long_wood_box.obj)", objshader);

	ModelManger::add_model(wood_box);

	vec3 light_coef(1.0, 0.007, 0.00028);

	vec3 white_light = vec3(20.0, 20.0, 20.0);
	LightManager::create_point_light(light_box, white_light, vec3(-1.0, 4.2, -13.8), vec3(1.0, 1.0, 10.0));
	LightManager::create_point_light(light_box, vec3(20.0, 0.0, 0.0), vec3(-1.1, 4.2, -10.9), vec3(1.0, 2.0, 2.0));
	LightManager::create_point_light(light_box, vec3(0.0, 20.0, 0.0), vec3(-1.1, 4.2, -7.9), vec3(1.0, 2.0, 2.0));
	LightManager::create_point_light(light_box, vec3(0.0, 0.0, 100.0), vec3(-1.1, 4.2, -3.9), vec3(1.0, 2.0, 2.0));

	unsigned int screenTex[2], depthRBO, FBO;
	unsigned int screenVAO, screenVBO;
	{
		float screen_box[] = {
		-1.0,-1.0,0.0,0.0,
		1.0,-1.0,1.0,0.0,
		1.0,1.0,1.0,1.0,

		-1.0,-1.0,0.0,0.0,
		1.0,1.0,1.0,1.0,
		-1.0,1.0,0.0,1.0
		};


		glGenVertexArrays(1, &screenVAO);
		glBindVertexArray(screenVAO);
		glGenBuffers(1, &screenVBO);
		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(screen_box), screen_box, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);

		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glGenTextures(2, screenTex);
		for (size_t i = 0; i < 2; i++) {
			glBindTexture(GL_TEXTURE_2D, screenTex[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, screenTex[i], 0);
		}
		GLenum bufs[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2,bufs);

		glGenRenderbuffers(1, &depthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Framebuffer didn't complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bool gamma = true;
	bool hdr = true;
	float exposure = 1.0;
	int tex_id = 0;
	float one_second = 0;
	int frame = 0;
	while (!glfwWindowShouldClose(w)) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		LightSettingUI::display();
		ImGui::Begin("hdr");
		ImGui::SliderFloat3("white light", value_ptr(LightManager::lights[0]->color), 1, 200, "%.f");
		ImGui::Checkbox("gamma", &gamma);
		ImGui::Checkbox("hdr", &hdr);
		if (hdr) {
			ImGui::SliderFloat("exposure", &exposure, 0.1, 10.0, "%.1f");
		}
		ImGui::SliderInt("base light bloom", &tex_id, 0, 1);
		ImGui::End();
		ImGui::Render();
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

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glClearColor(background_color.r, background_color.g, background_color.b, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LightManager::draw();
		LightManager::update_lighting_info_in_obj_shader(objshader);

		objshader->use();
		objshader->set_matrix("view", cam.get_view_matrix());
		objshader->set_matrix("projection", perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f));
		objshader->set_uniform_3fv("eye_pos", cam.cam_pos);

		ModelManger::draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		screen_shader->use();
		screen_shader->set_uniform_1b("use_gamma", gamma);
		screen_shader->set_uniform_1b("use_hdr", hdr);
		screen_shader->set_uniform_1f("exposure", exposure);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTex[tex_id]);
		screen_shader->set_texture("screenTexture", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, screenTex[1]);
		screen_shader->set_texture("bloomTexture", 1);
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	objshader->delete_program();
	lightingshader->delete_program();
	delete objshader;
	delete lightingshader;
	LightManager::destory_all_lights();
	ModelManger::destroy_all_models();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
//}