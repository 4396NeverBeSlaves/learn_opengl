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


namespace shadow_mapping_imgui {

	using namespace std;
	using namespace glm;

	vector<Texture> textures;
	vec3 background_color = vec3(0.2, 0.2, 0.2);
	Shader* lightingshader;
	Model* light_box;

	const int WIDTH = 900;
	const int HEIGHT = 900;
	vec3 cam_pos(0, 2, 0);
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

		lightingshader = new Shader("shadow_mapping_light_shader.vert", "shadow_mapping_light_shader.frag");
		light_box = new Model(R"(C:\Users\X\Desktop\box.obj)", lightingshader);

		Shader* objshader = new Shader("shadow_mapping_obj_shader.vert", "shadow_mapping_obj_shader.frag");
		Shader* depthshader = new Shader("shadow_mapping_depth_shader.vert", "shadow_mapping_depth_shader.frag");
		Shader* screenshader = new Shader("shadow_mapping_screen_shader.vert", "shadow_mapping_screen_shader.frag");

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

		LightManager::create_direction_light(lightingshader, vec3(1.0, 1.0, 1.0), vec3(-0.5, -0.5, -0.8));
		//LightManager::create_point_light(light_box, vec3(1.0, 1.0, 1.0), vec3(-5, 11, -6), light_coef);
		//LightManager::create_point_light(light_box, vec3(0.0, 1.0, 0.0), vec3(6.6, 13.3, 2.5), light_coef);
		//LightManager::create_point_light(light_box, vec3(0.0, 0.0, 1.0), vec3(0.0, 3.0, -10.0), light_coef);
		//LightManager::create_point_light(light_box, vec3(1.0, 0.0, 0.0), vec3(-3, 2.6, 6.5), light_coef);
		//LightManager::create_spot_light(light_box, vec3(1.0, 1.0, 1.0), vec3(2.0, 0.6, 3.2), light_coef, vec3(0.0, 0.0, -1.0), 15.0, 17.0);

		float screen_box[] = {
			0.0,0.0,0.0,0.0,
			1.0,0.0,1.0,0.0,
			1.0,1.0,1.0,1.0,

			0.0,0.0,0.0,0.0,
			1.0,1.0,1.0,1.0,
			0.0,1.0,0.0,1.0
		};

		unsigned int screenVAO, screenVBO;
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


		unsigned int depthFBO, depthTex;
		glGenFramebuffers(1, &depthFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
		glGenTextures(1, &depthTex);
		glBindTexture(GL_TEXTURE_2D, depthTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			cout << "Framebuffer didn't complete!" << endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		float one_second = 0;
		int frame = 0;

		while (!glfwWindowShouldClose(w)) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			LightSettingUI::display();
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

			glViewport(0, 0, 1024, 1024);
			glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
			glClearColor(0.0, 0.0, 0.0, 1);
			glClear(GL_DEPTH_BUFFER_BIT);

			mat4 view = lookAt(vec3(0, 0, 0), static_cast<DirectionLight*>(LightManager::lights[0])->direction, vec3(0, 1, 0));
			mat4 proj = ortho(-25.0f, 25.0f, -10.0f, 10.0f, -20.0f, 20.0f);
			mat4 lighting_matrix = proj * view;
			depthshader->use();
			depthshader->set_matrix("view", view);
			depthshader->set_matrix("projection", proj);
			ModelManger::change_shader(depthshader);

			ModelManger::models[1]->translate(vec3(-1.0, 0, -2.3));
			ModelManger::models[2]->translate(vec3(2, 2, 2));
			ModelManger::models[3]->rotate(radians(current_time * 20), vec3(0, 1, 0));
			ModelManger::models[3]->translate(vec3(-2.5, 0, 2.5));
			ModelManger::draw();


			glViewport(0, 0, WIDTH, HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(0.0, 0.0, 0.0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			screenshader->use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depthTex);
			screenshader->set_texture("shadowmap", 0);
			glBindVertexArray(screenVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			LightManager::draw();
			LightManager::update_lighting_info_in_obj_shader(objshader);


			objshader->use();
			objshader->set_matrix("view", cam.get_view_matrix());
			objshader->set_matrix("projection", perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f));
			objshader->set_uniform_3fv("eye_pos", cam.cam_pos);
			objshader->set_matrix("lighting_matrix", lighting_matrix);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, depthTex);
			objshader->set_texture("shadowmap", 2);
			ModelManger::change_shader(objshader);

			ModelManger::models[1]->translate(vec3(-1.0, 0, -2.3));
			ModelManger::models[2]->translate(vec3(2, 2, 2));
			ModelManger::models[3]->rotate(radians(current_time * 20), vec3(0, 1, 0));
			ModelManger::models[3]->translate(vec3(-2.5, 0, 2.5));
			ModelManger::draw();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(w);
			glfwPollEvents();
		}

		objshader->delete_program();
		lightingshader->delete_program();
		delete objshader;
		delete lightingshader;
		delete depthshader;
		delete screenshader;
		LightManager::destory_all_lights();
		ModelManger::destroy_all_models();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
		return 0;
	}

}