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
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDepthFunc(GL_LESS);
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(w, framebuf_size_callback);
	glfwSetKeyCallback(w, key_callback);
	glfwSetScrollCallback(w, scroll_callback);
	glfwSetCursorPosCallback(w, cursor_callback);

	float points[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,5*sizeof(float), (void*)(2*sizeof(float)));
	glBindVertexArray(0);

	Shader s("geometry_shader08_obj_shader.vert","geometry_shader08_obj_shader.geom", "geometry_shader08_obj_shader.frag");

	float one_second = 0;
	int frame = 0;
	while (!glfwWindowShouldClose(w)) {
		delta_time = get_delta_time();
		one_second += delta_time;
		frame++;
		if (one_second >= 1.0) {
			string title = format("Geometry shader. [{:6.1f}FPS, {:5.2f}ms] [FOV: {:4.1f}] [Yaw:{:7.1f}, Pitch:{:5.1f}] [Position:{:5.1f} {:5.1f} {:5.1f}, Direction:{:4.1f} {:4.1f} {:4.1f}]",
				frame / one_second, one_second / frame * 1000, cam.fov, cam.yaw_, cam.pitch_, cam.cam_pos.x, cam.cam_pos.y, cam.cam_pos.z, cam.cam_dir.x, cam.cam_dir.y, cam.cam_dir.z);
			glfwSetWindowTitle(w, title.c_str());
			one_second = 0.0;
			frame = 0;
		}
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		s.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS,0,4);



		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	//glDeleteBuffers(1, &vbo);
	//glDeleteVertexArrays(1, &vao);
	s.delete_program();
	glfwTerminate();
	return 0;
}