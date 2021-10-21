#include<iostream>
#include<format>
#include<vector>
#include<random>
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

//namespace _SSAO_{
using namespace std;
using namespace glm;

vector<Texture> textures;
vec3 background_color = vec3(0.0, 0.0, 0.0);
Shader* lightingshader;
Model* light_box;

const int WIDTH = 900;
const int HEIGHT = 900;
vec3 cam_pos(0, 10, 4.6);
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

	lightingshader = new Shader(R"(..\4 Advanced_Lighting\SSAO_light_shader.vert)", R"(..\4 Advanced_Lighting\SSAO_light_shader.frag)");
	light_box = new Model(R"(..\Assets\box.obj)", lightingshader);


	Shader* gbuffer_shader = new Shader(R"(..\4 Advanced_Lighting\SSAO_gbuffer_shader.vert)", R"(..\4 Advanced_Lighting\SSAO_gbuffer_shader.frag)");
	Shader* ssao_shader=new Shader(R"(..\4 Advanced_Lighting\SSAO_ssao_shader.vert)", R"(..\4 Advanced_Lighting\SSAO_ssao_shader.frag)");
	Shader* ssao_blur_shader=new Shader(R"(..\4 Advanced_Lighting\SSAO_ssao_blur_shader.vert)", R"(..\4 Advanced_Lighting\SSAO_ssao_blur_shader.frag)");
	Shader* lighting_pass_shader = new Shader(R"(..\4 Advanced_Lighting\SSAO_lighting_pass_shader.vert)", R"(..\4 Advanced_Lighting\SSAO_lighting_pass_shader.frag)");


	Model* nanosuit = new Model(R"(..\Assets\nanosuit\nanosuit.obj)", gbuffer_shader);
	Model* plane = new Model(R"(..\Assets\plane.obj)", gbuffer_shader);
	ModelManger::add_model(nanosuit);
	ModelManger::add_model(plane);

	vec3 light_coef(1.0, 0.027, 0.0028);

	vec3 white_light = vec3(4.0, 4.0, 4.0);
	LightManager::create_point_light(light_box, white_light, vec3(7.3, 2.8, -12), light_coef);
	//LightManager::create_point_light(light_box, vec3(1.0, 0.0, 0.0), vec3(7.0, 10.0, 4.0), light_coef);
	//LightManager::create_point_light(light_box, vec3(0.0, 1.0, 0.0), vec3(14.0, 10.0, 4.0), light_coef);
	//LightManager::create_point_light(light_box, vec3(0.0, 0.0, 1.0), vec3(21.0, 10.0, 4.0), light_coef);

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

		glBindTexture(GL_TEXTURE_2D, gbuffer[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbuffer[0], 0);
		
		glBindTexture(GL_TEXTURE_2D, gbuffer[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbuffer[1], 0);

		glBindTexture(GL_TEXTURE_2D, gbuffer[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

	unsigned int SSAO_framebuffer, ssao_tex;
	{
		glGenFramebuffers(1, &SSAO_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, SSAO_framebuffer);
		glGenTextures(1, &ssao_tex);
		glBindTexture(GL_TEXTURE_2D, ssao_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_tex, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Framebuffer didn't complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	unsigned int SSAOblur_framebuffer, ssaoblur_tex;
	{
		glGenFramebuffers(1, &SSAOblur_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, SSAOblur_framebuffer);
		glGenTextures(1, &ssaoblur_tex);
		glBindTexture(GL_TEXTURE_2D, ssaoblur_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoblur_tex, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Framebuffer didn't complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uniform_real_distribution<float> random_float(0.0, 1.0);
	default_random_engine eng;
	vector<vec3> sampling_kernel;
	vector<vec3> noises;
	const int sampling_times = 64;
	for (int i = 0; i < sampling_times; i++) {
		vec3 sample = vec3(random_float(eng)*2.0-1.0, random_float(eng) * 2.0 - 1.0, random_float(eng));
		sample = normalize(sample);
		sample *= random_float(eng);
		float scale = i * 1.0 / sampling_times;
		scale = 0.1 + scale * scale * 0.9;
		sample *= scale;
		sampling_kernel.push_back(sample);
	}
	for (size_t i = 0; i < 16; i++) {
		vec3 noise = vec3(random_float(eng)*2.0-1.0, random_float(eng) * 2.0 - 1.0, 0.0);
		noises.push_back(noise);
	}
	unsigned int noises_texture;
	glGenTextures(1, &noises_texture);
	glBindTexture(GL_TEXTURE_2D, noises_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &noises[0]);
	glBindTexture(GL_TEXTURE_2D,0);
		
	bool gamma = false;
	bool hdr = false;
	float exposure = 1.0;
	bool check_texture = false;
	int tex_id = 0;
	bool use_AO = false;
	bool smooth_edge = false;
	float sampling_radius = 1.0;
	vec3 nanosuit_t(0,1.5,0);
	float nanosuit_r = -90;
	float one_second = 0;
	int frame = 0;
	while (!glfwWindowShouldClose(w)) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		LightSettingUI::display();
		ImGui::Begin("hdr");
		ImGui::Checkbox("gamma", &gamma);
		ImGui::Checkbox("hdr", &hdr);
		if (hdr) {
			ImGui::SliderFloat("exposure", &exposure, 0.1, 10.0, "%.1f");
		}
		ImGui::Checkbox("check_texture", &check_texture);
		if (check_texture) {
			ImGui::Separator();
			ImGui::RadioButton("ssao ", &tex_id, 0);
			ImGui::RadioButton("ssao blur", &tex_id, 1);
			ImGui::RadioButton("gbuffer position", &tex_id, 2);
			ImGui::RadioButton("gbuffer normal", &tex_id, 3);
			ImGui::RadioButton("gbuffer albedo & specular", &tex_id, 4);
			ImGui::Separator();
		}
		ImGui::Checkbox("AO", &use_AO);
		ImGui::SliderFloat("sampling_radius", &sampling_radius, 0.1, 5, "%.1f");
		ImGui::Checkbox("smooth_edge", &smooth_edge);
		ImGui::SliderFloat("nanosuit pos y", &nanosuit_t.y,-2,2,"%.2f");
		ImGui::SliderFloat("nanosuit rotate", &nanosuit_r,-180,180,"%.f");
		ModelManger::models[0]->translate(nanosuit_t);
		ModelManger::models[0]->rotate(radians(nanosuit_r), vec3(1,0,0));
		ImGui::End();
		ImGui::Render();
		delta_time = get_delta_time();
		one_second += delta_time;
		frame++;
		if (one_second >= 1.0) {
			string title = format("SSAO. [{:6.1f}FPS, {:5.2f}ms] [FOV: {:4.1f}] [Yaw:{:7.1f}, Pitch:{:5.1f}] [Position:{:5.1f} {:5.1f} {:5.1f}, Direction:{:4.1f} {:4.1f} {:4.1f}]",
				frame / one_second, one_second / frame * 1000, cam.fov, cam.yaw_, cam.pitch_, cam.cam_pos.x, cam.cam_pos.y, cam.cam_pos.z, cam.cam_dir.x, cam.cam_dir.y, cam.cam_dir.z);
			glfwSetWindowTitle(w, title.c_str());
			one_second = 0.0;
			frame = 0;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, gframebuffer);
		glClearColor(background_color.r, background_color.g, background_color.b, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		gbuffer_shader->use();
		gbuffer_shader->set_matrix("view", cam.get_view_matrix());
		gbuffer_shader->set_matrix("projection", perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f));
		gbuffer_shader->set_uniform_3fv("eye_pos", cam.cam_pos);
		ModelManger::draw();

		glBindFramebuffer(GL_FRAMEBUFFER, SSAO_framebuffer);
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT );

		ssao_shader->use();
		ssao_shader->set_matrix("projection", perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f));
		ssao_shader->set_uniform_1b("smooth_edge", smooth_edge);
		ssao_shader->set_uniform_1f("sampling_radius", sampling_radius);
		for (size_t i = 0; i < sampling_times; i++) {
			ssao_shader->set_uniform_3fv(format("sampling_kernel[{:d}]",i), sampling_kernel[i]);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer[0]);
		ssao_shader->set_texture("gPosition", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gbuffer[1]);
		ssao_shader->set_texture("gNormal", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noises_texture);
		ssao_shader->set_texture("noises", 2);
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		glBindFramebuffer(GL_FRAMEBUFFER, SSAOblur_framebuffer);
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		ssao_blur_shader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssao_tex);
		ssao_blur_shader->set_texture("ssaoTex", 0);
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lighting_pass_shader->use();
		//lighting_pass_shader->set_uniform_3fv("eye_pos", cam.cam_pos);
		lighting_pass_shader->set_uniform_1b("use_gamma", gamma);
		lighting_pass_shader->set_uniform_1b("use_hdr", hdr);
		lighting_pass_shader->set_uniform_1b("use_AO", use_AO);
		lighting_pass_shader->set_uniform_1f("exposure", exposure);
		lighting_pass_shader->set_uniform_1f("check_texture", check_texture);
		lighting_pass_shader->set_uniform_1int("texture_id", tex_id);
		lighting_pass_shader->set_matrix("view_matrix", cam.get_view_matrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer[0]);
		lighting_pass_shader->set_texture("gPosition", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gbuffer[1]);
		lighting_pass_shader->set_texture("gNormal", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gbuffer[2]);
		lighting_pass_shader->set_texture("gAlbedoSpecular", 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, ssao_tex);
		lighting_pass_shader->set_texture("ssaoTexture", 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ssaoblur_tex);
		lighting_pass_shader->set_texture("ssao_blur_Texture", 4);
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, gframebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		LightManager::draw();
		LightManager::update_lighting_info_in_obj_shader(lighting_pass_shader);

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