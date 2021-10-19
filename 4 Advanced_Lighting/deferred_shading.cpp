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

//namespace deferred_shading_{
using namespace std;
using namespace glm;

vector<Texture> textures;
vec3 background_color = vec3(0.0, 0.0, 0.0);
Shader* lightingshader;
Model* light_box;

const int WIDTH = 900;
const int HEIGHT = 900;
vec3 cam_pos(10, 10, 40);
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

	//glfwSwapInterval(1);
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

	lightingshader = new Shader(R"(..\4 Advanced_Lighting\deferred_shading_light_shader.vert)", R"(..\4 Advanced_Lighting\deferred_shading_light_shader.frag)");
	light_box = new Model(R"(..\Assets\box.obj)", lightingshader);

	Shader* obj_shader = new Shader(R"(..\4 Advanced_Lighting\deferred_shading_obj_shader.vert)", R"(..\4 Advanced_Lighting\deferred_shading_obj_shader.frag)");
	Shader* screen_shader = new Shader(R"(..\4 Advanced_Lighting\deferred_shading_screen_shader.vert)", R"(..\4 Advanced_Lighting\deferred_shading_screen_shader.frag)");

	Shader* gbuffer_shader = new Shader(R"(..\4 Advanced_Lighting\deferred_shading_gbuffer_shader.vert)", R"(..\4 Advanced_Lighting\deferred_shading_gbuffer_shader.frag)");
	Shader* lighting_pass_shader = new Shader(R"(..\4 Advanced_Lighting\deferred_shading_lighting_pass_shader.vert)", R"(..\4 Advanced_Lighting\deferred_shading_lighting_pass_shader.frag)");
	
	Shader* shader0 = obj_shader;
	Shader* shader1 = screen_shader;

	//Model* nanosuit = new Model(R"(..\Assets\nanosuit\nanosuit.obj)", shader0);

	for (size_t i = 0; i < 9; i++) {
		ModelManger::add_model(new Model(R"(..\Assets\nanosuit\nanosuit.obj)", shader0));
	}
	
	vec3 light_coef(1.0, 0.007, 0.00028);

	vec3 white_light = vec3(1.0, 1.0, 1.0);
	LightManager::create_point_light(light_box, white_light, vec3(0.0, 10.0, 4.0), light_coef);
	LightManager::create_point_light(light_box, vec3(1.0, 0.0, 0.0), vec3(7.0, 10.0, 4.0), light_coef);
	LightManager::create_point_light(light_box, vec3(0.0, 1.0, 0.0), vec3(14.0, 10.0, 4.0), light_coef);
	LightManager::create_point_light(light_box, vec3(0.0, 0.0, 1.0), vec3(21.0, 10.0, 4.0), light_coef);

	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			for (size_t k = 0; k < 3; k++) {
				vec3 c;
				float t = glfwGetTime();
				c.x = sin(radians(t * 10000+ i)) / 2 + 0.5;
				c.y = sin(radians(t * 10000+ 120 + j)) / 2 + 0.5;
				c.z = sin(radians(t * 10000+ 240 + k)) / 2 + 0.5;
				cout <<t<<" "<< c.x << " " << c.y << " " << c.z << endl;
				vec3 p = vec3(-6, -2, -6) + vec3(i * 10, j * 10, k * 10);;
				LightManager::create_point_light(light_box, c * 1.0f, p, light_coef);
			}
		}
	}
	unsigned int gbuffer[3], depthRBO, gframebuffer;
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

		glGenFramebuffers(1, &gframebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gframebuffer);
		glGenTextures(3, gbuffer);
		for (size_t i = 0; i < 2; i++) {
			glBindTexture(GL_TEXTURE_2D, gbuffer[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gbuffer[i], 0);
		}
		glBindTexture(GL_TEXTURE_2D, gbuffer[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gbuffer[2], 0);
		GLenum bufs[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, bufs);

		glGenRenderbuffers(1, &depthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Framebuffer didn't complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bool gamma = false;
	bool hdr = false;
	float exposure = 1.0;
	bool check_gbuffer = false;
	int tex_id = 0;
	bool use_forward_shading = true;
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
		ImGui::Checkbox("check gbuffer", &check_gbuffer);
		if(check_gbuffer){
			ImGui::Separator();
			ImGui::RadioButton("gbuffer position", &tex_id, 0);
			ImGui::RadioButton("gbuffer normal", &tex_id, 1);
			ImGui::RadioButton("gbuffer albedo & specular", &tex_id, 2);
			ImGui::Separator();
		}
		ImGui::Checkbox("forward shading", &use_forward_shading);
		if (use_forward_shading) {
			lightingshader->use();
			lightingshader->set_uniform_1b("forward_shading", true);
			shader0 = obj_shader;
			shader1 = screen_shader;
		}
		else {
			lightingshader->use();
			lightingshader->set_uniform_1b("forward_shading", false);
			shader0 = gbuffer_shader;
			shader1 = lighting_pass_shader;
		}
		for (auto& m : ModelManger::models) {
			m->shader = shader0;
		}
		ImGui::End();
		ImGui::Render();
		delta_time = get_delta_time();
		one_second += delta_time;
		frame++;
		if (one_second >= 1.0) {
			string title = format("Deferred Shading. [{:6.1f}FPS, {:5.2f}ms] [FOV: {:4.1f}] [Yaw:{:7.1f}, Pitch:{:5.1f}] [Position:{:5.1f} {:5.1f} {:5.1f}, Direction:{:4.1f} {:4.1f} {:4.1f}]",
				frame / one_second, one_second / frame * 1000, cam.fov, cam.yaw_, cam.pitch_, cam.cam_pos.x, cam.cam_pos.y, cam.cam_pos.z, cam.cam_dir.x, cam.cam_dir.y, cam.cam_dir.z);
			glfwSetWindowTitle(w, title.c_str());
			one_second = 0.0;
			frame = 0;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, gframebuffer);
		glClearColor(background_color.r, background_color.g, background_color.b, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LightManager::draw();
		if (use_forward_shading)
			LightManager::update_lighting_info_in_obj_shader(shader0);
		else
			LightManager::update_lighting_info_in_obj_shader(shader1);

		shader0->use();
		shader0->set_matrix("view", cam.get_view_matrix());
		shader0->set_matrix("projection", perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f));
		shader0->set_uniform_3fv("eye_pos", cam.cam_pos);
		for (size_t i = 0; i < 9; i++) {
			ModelManger::models[i]->translate(vec3((i % 3)*10,0.0, (i / 3)*10));
		}
		ModelManger::draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader1->use();
		shader1->set_uniform_3fv("eye_pos", cam.cam_pos);
		shader1->set_uniform_1b("use_gamma", gamma);
		shader1->set_uniform_1b("use_hdr", hdr);
		shader1->set_uniform_1f("exposure", exposure);
		shader1->set_uniform_1f("check_gbuffer", check_gbuffer);
		shader1->set_uniform_1int("gbuffer_id", tex_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer[0]);
		shader1->set_texture("gPosition", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gbuffer[1]);
		shader1->set_texture("gNormal", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gbuffer[2]);
		shader1->set_texture("gAlbedoSpecular", 2);
		


		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	gbuffer_shader->delete_program();
	lightingshader->delete_program();
	delete gbuffer_shader;
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