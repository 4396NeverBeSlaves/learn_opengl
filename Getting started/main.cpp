#include<iostream>
#include<format>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"shader.h"
#include"stb_image.h"
#include"camera.h"

using namespace std;
using namespace glm;

const int WIDTH = 900;
const int HEIGHT = 900;
vec3 cam_pos(0, 0, 3);
vec3 cam_dir(0, 0, -1);
vec3 cam_up(0, 1, 0);
Camera cam(cam_pos,cam_dir,cam_up);

void framebuf_size_callback(GLFWwindow *w,int width,int height) {
	glViewport(0, 0, width, height);
	int cw, ch;
	glfwGetFramebufferSize(w, &cw, &ch);
	cout << "frame buf w:" << cw << " h:" << ch << endl;
	
	cout << "resizing window: w:" << width << " h: " << height << endl<< endl;
}

float current_time=0.0;
float old_time;
float delta_time;
float get_delta_time() {
	old_time = current_time;
	current_time = glfwGetTime();
	return current_time - old_time;
}

float tranparency = 0.5;
void key_callback(GLFWwindow* w,int key,int scannode,int action,int mods) {
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

	GLFWwindow* w = glfwCreateWindow(WIDTH, HEIGHT, "an opengl program", NULL, NULL);
	if (!w) {
		cout << "glfwCreateWindow failed!" << endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(w);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "failed to init glad!" << endl;
		return -1;
	}

	glViewport(0,0,WIDTH,HEIGHT);

	glfwSetFramebufferSizeCallback(w, framebuf_size_callback);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader s("vertex shader.vert", "frag shader.frag");

	//float vertices[] = {
	////vertex		color				uv 
	//-0.5,-0.5,0.0,	1.0,0.0,0.0,		0.0,0.0,	//left bottom
	//0.5,-0.5,0.0,	0.0,1.0,0.0,		1.0,0.0,	//right bottom
	//-0.5,0.5,0.0,	0.0,0.0,1.0,		0.0,1.0,	//left up
	//0.5,0.5,0.0,	1.0,0.0,0.0,		1.0,1.0		//right up
	//};
	//unsigned int idxs[] = {
	//	0,1,2,
	//	1,2,3
	//};
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
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo_id;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//unsigned int ebo;
	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxs), idxs, GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	int width_texture, height_texture, nchannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char * data=stbi_load("container.jpg", &width_texture, &height_texture, &nchannels, 0);

	if (!data) {
		cerr << "texture file not found!" << endl;
		return -1;
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width_texture,height_texture,0,GL_RGB,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);


	unsigned char* data1 = stbi_load("awesomeface.png", &width_texture, &height_texture, &nchannels, 0);

	if (!data1) {
		cerr << "texture file not found!" << endl;
		return -1;
	}

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_texture, height_texture, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data1);

	s.use();
	s.set_texture("myTexture0", 0);
	s.set_texture("myTexture1", 1);


	while (!glfwWindowShouldClose(w) )	{

		delta_time = get_delta_time();
		cout << format("lag: {:4.2f}ms",delta_time*1000) << endl;

		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


		//unsigned int color_uni = glGetUniformLocation(shader_prog, "color");
		
		//cout << time << endl;
		//s.set_uniform_float("color", sin(time) / 2.0 + 0.5, sin(time + 2.0 / 3.0 * 3.1415) / 2.0 + 0.5, sin(time + 4.0 / 3.0 * 3.1415) / 2.0 + 0.5);
		//glUniform3f(color_uni, sin(time)/2.0+0.5, sin(time+2.0/3.0*3.1415)/2.0+0.5, sin(time + 4.0 / 3.0 * 3.1415) / 2.0 + 0.5);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(vao);
		s.use();

		glfwSetKeyCallback(w,key_callback);
		glfwSetCursorPosCallback(w, cursor_callback);
		glfwSetScrollCallback(w, scroll_callback);

		//cout << time<<"\ttransp:"<<tranparency << endl;
		s.set_uniform_1f("tranparency", tranparency);

		//mat4x4 trans = mat4x4(1.0f);
		//
		//trans = translate(trans, vec3(0.5, 0.5, 0));
		//trans = rotate(trans, (float)time, vec3(0, 0, 1));
		//trans = scale(trans, vec3(0.5, 0.5, 0.5));
		//
		//s.set_matrix("transform", trans);

		
		mat4 view = cam.get_view_matrix();
		mat4 proj = perspective((float)radians(cam.fov), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);
		s.set_matrix("view", view);
		s.set_matrix("projection", proj);
		
		//glDrawElements(GL_TRIANGLES,sizeof(idxs),GL_UNSIGNED_INT,0);
		
		for (int i = 0; i < 10; i++) {
			mat4 model = mat4(1.0f);
			model = translate(model, cubePositions[i]);
			model = rotate(model, (float)radians(20.0f*(i+1)) * current_time, vec3(0.5f, 1.0f, 0.0f));
			
			s.set_matrix("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(w);
		glfwPollEvents();

	}
	s.delete_program();
	glDeleteBuffers(1, &vbo_id);
	//glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();

	return 0;
}