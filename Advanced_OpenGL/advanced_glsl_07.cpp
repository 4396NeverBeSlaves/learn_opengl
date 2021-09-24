#include<iostream>
#include<format>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"Camera.h"
#include"Shader.h"
#include"Texture.h"
#include"../stb_image.h"

namespace advanced_glsl07{

using namespace std;
using namespace glm;



const int WIDTH = 900;
const int HEIGHT = 900;
vec3 cam_pos(0, 0, 2);
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
	GLFWwindow* w = glfwCreateWindow(WIDTH, HEIGHT, "My opengl program.", NULL, NULL);
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
	glDepthFunc(GL_LESS);
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(w, framebuf_size_callback);
	glfwSetKeyCallback(w, key_callback);
	glfwSetScrollCallback(w, scroll_callback);
	glfwSetCursorPosCallback(w, cursor_callback);
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	float planeVertices[] = {
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	Texture cube_texture("marble.jpg", true);
	Texture plane_texture("metal.png", true);
	Texture container_texture("container.jpg", true);

	Shader s("advancedglsl07_obj_shader.vert", "advancedglsl07_obj_shader.frag");
	Shader redbox_shader("advancedglsl07_4colorboxes_shader.vert", "advancedglsl07_redbox_shader.frag");
	Shader greenbox_shader("advancedglsl07_4colorboxes_shader.vert", "advancedglsl07_greenbox_shader.frag");
	Shader bluebox_shader("advancedglsl07_4colorboxes_shader.vert", "advancedglsl07_bluebox_shader.frag");
	Shader whitebox_shader("advancedglsl07_4colorboxes_shader.vert", "advancedglsl07_whitebox_shader.frag");

	unsigned int UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
	glUniformBlockBinding(s.id, glGetUniformBlockIndex(s.id, "myMatrices"),0);
	glUniformBlockBinding(redbox_shader.id, glGetUniformBlockIndex(redbox_shader.id, "myMatrices"),0);
	glUniformBlockBinding(greenbox_shader.id, glGetUniformBlockIndex(greenbox_shader.id, "myMatrices"),0);
	glUniformBlockBinding(bluebox_shader.id, glGetUniformBlockIndex(bluebox_shader.id, "myMatrices"),0);
	glUniformBlockBinding(whitebox_shader.id, glGetUniformBlockIndex(whitebox_shader.id, "myMatrices"),0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	float one_second = 0;
	int frame = 0;
	while (!glfwWindowShouldClose(w)) {
		delta_time = get_delta_time();
		one_second += delta_time;
		frame++;
		if (one_second >= 1.0) {
			string title = format("Advanced GLSL. [{:6.1f}FPS, {:5.2f}ms] [FOV: {:4.1f}] [Yaw:{:7.1f}, Pitch:{:5.1f}] [Position:{:5.1f} {:5.1f} {:5.1f}, Direction:{:4.1f} {:4.1f} {:4.1f}]",
				frame / one_second, one_second / frame * 1000, cam.fov, cam.yaw_, cam.pitch_, cam.cam_pos.x, cam.cam_pos.y, cam.cam_pos.z, cam.cam_dir.x, cam.cam_dir.y, cam.cam_dir.z);
			glfwSetWindowTitle(w, title.c_str());
			one_second = 0.0;
			frame = 0;
		}
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		mat4 view = cam.get_view_matrix();
		mat4 proj = perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
		mat4 model = mat4(1.0);
		s.use();
		s.set_matrix("model", model);
		glBindBuffer(GL_UNIFORM_BUFFER,UBO);
		glBufferSubData(GL_UNIFORM_BUFFER,0,64,value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, value_ptr(proj));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plane_texture.get_texture_obj());
		glBindVertexArray(planeVAO);
		s.use();
		s.set_texture("diffusemap", 0);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(planeVertices) / 5);
		glBindVertexArray(0);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture.get_texture_obj());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, container_texture.get_texture_obj());
		s.use();
		s.set_texture("diffusemap", 0);
		s.set_texture("diffusemap1", 1);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / 5);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture.get_texture_obj());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, container_texture.get_texture_obj());
		s.use();
		s.set_matrix("model", translate(mat4(1.0), vec3(-2.8, 0, 2.5)));
		s.set_texture("diffusemap", 0);
		s.set_texture("diffusemap1", 1);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / 5);
		glBindVertexArray(0);

		redbox_shader.use();
		redbox_shader.set_matrix("model", translate(mat4(1.0), vec3(3, 3, 3)));
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / 5);
		glBindVertexArray(0);

		greenbox_shader.use();
		greenbox_shader.set_matrix("model", translate(mat4(1.0), vec3(3, 3, 4.5)));
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / 5);
		glBindVertexArray(0);

		bluebox_shader.use();
		bluebox_shader.set_matrix("model", translate(mat4(1.0), vec3(3, 1.5, 3)));
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / 5);
		glBindVertexArray(0);		
		
		whitebox_shader.use();
		whitebox_shader.set_matrix("model", translate(mat4(1.0), vec3(3, 1.5, 4.5)));
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / 5);
		glBindVertexArray(0);

		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	//glDeleteBuffers(1, &vbo);
	//glDeleteVertexArrays(1, &vao);
	s.delete_program();
	glfwTerminate();
	return 0;
}}