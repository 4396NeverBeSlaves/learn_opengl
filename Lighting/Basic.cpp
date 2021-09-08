#include<iostream>
#include<format>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"camera.h"
#include"shader.h"
#include"texture.h"
#include"stb_image.h"

using namespace std;
using namespace glm;

namespace Basic{

	const int WIDTH = 900;
	const int HEIGHT = 900;
	vec3 cam_pos(0, 0, 3);
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

	int main1() {
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
		glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetFramebufferSizeCallback(w, framebuf_size_callback);
		glfwSetKeyCallback(w, key_callback);
		glfwSetScrollCallback(w, scroll_callback);
		glfwSetCursorPosCallback(w, cursor_callback);

		float vertices[] = {
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
		vec3 cubePositions[] = {
			vec3(0.0f,  0.0f,  0.0f),
			vec3(2.0f,  5.0f, -15.0f),
			vec3(-1.5f, -2.2f, -2.5f),
			vec3(-3.8f, -2.0f, -12.3f),
			vec3(2.4f, -0.4f, -3.5f),
			vec3(-1.7f,  3.0f, -7.5f),
			vec3(1.3f, -2.0f, -2.5f),
			vec3(1.5f,  2.0f, -2.5f),
			vec3(1.5f,  0.2f, -1.5f),
			vec3(-1.3f,  1.0f, -1.5f)
		};

		unsigned int vao, vbo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		Texture texture0("awesomeface.png", true);
		Texture texture1("container.jpg", true);

		Shader s("vertex shader.vert", "frag shader.frag");
		s.use();
		s.set_texture("myTexture0", 0);
		s.set_texture("myTexture1", 1);

		while (!glfwWindowShouldClose(w)) {
			delta_time = get_delta_time();
			//cout << format("lag: {:4.2f}ms", delta_time * 1000) << endl;

			glClearColor(0.2, 0.2, 0.2, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			mat4 view = cam.get_view_matrix();
			mat4 proj = perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
			s.set_matrix("view", view);
			s.set_matrix("projection", proj);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture0.get_texture_obj());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture1.get_texture_obj());
			glBindVertexArray(vao);

			for (int i = 0; i < 10; i++) {
				mat4 model = mat4(1.0);
				model = translate(model, cubePositions[i]);
				model = rotate(model, (float)radians((i + 1) * 20.0) * current_time, vec3(1, 0, 0));
				s.set_matrix("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			glfwSwapBuffers(w);
			glfwPollEvents();
		}

		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		s.delete_program();
		glfwTerminate();
		return 0;
	}
}