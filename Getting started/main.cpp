#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"shader.h"
#include"stb_image.h"

using namespace std;
using namespace glm;

const int WIDTH = 1200;
const int HEIGHT = 1200;

void framebuf_size_callback(GLFWwindow *w,int width,int height) {
	glViewport(0, 0, width, height);
	int cw, ch;
	glfwGetFramebufferSize(w, &cw, &ch);
	cout << "frame buf w:" << cw << " h:" << ch << endl;
	
	cout << "resizing window: w:" << width << " h: " << height << endl<< endl;
}

void process_input(GLFWwindow *w) {
	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(w, true);
	}
}

float tranparency = 0.5;
void key_callback(GLFWwindow* w,int key,int scannode,int action,int mods) {
	if (glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS) {
		if (tranparency < 1.0)
			tranparency += 0.01;
	}
	if (glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (tranparency > 0.1)
			tranparency -= 0.01;
	}
}
void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}



int main() {
	/*vec4 v(1, 0, 0, 1);
	mat4x4 trans(1.0);
	trans=translate(trans, vec3(1.0, 1.0, 0.0));
	v = trans * v;
	cout << v.x<<v.y<<v.z<<v.w << endl;*/

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



	//const char* vertex_shader_source = "#version 330 core\n"
	//	"layout (location =0) in vec3 aPos;\n"
	//	"layout (location=1) in vec3 aColor;\n"
	//	"out vec3 color;\n"
	//	"void main(){\n"
	//	"	gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
	//	"	color=aColor;"
	//	"};\n";
	//unsigned int vertex_shader;
	//vertex_shader=glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	//glCompileShader(vertex_shader);
	//check_shader(vertex_shader);

	//const char* frag_shader_source="#version 330 core\n"
	//	"//in vec3 color;\n"
	//	"uniform vec3 color;\n"
	//	"out vec4 FragColor;\n"
	//	"void main(){\n"
	//	"	FragColor=vec4(color,1.0);"
	//	"}";
	//unsigned int frag_shader;
	//frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
	//glCompileShader(frag_shader);
	//check_shader(frag_shader);

	//unsigned int shader_prog;
	//shader_prog = glCreateProgram();
	//glAttachShader(shader_prog, vertex_shader);
	//glAttachShader(shader_prog, frag_shader);
	//glLinkProgram(shader_prog);
	//check_program(shader_prog);

	//glDeleteShader(vertex_shader);
	//glDeleteShader(frag_shader);

	Shader s("vertex shader.vert", "frag shader.frag");

	float vertices[] = {
	-0.5,-0.5,0.0,	1.0,0.0,0.0,		0.0,0.0,	//left bottom
	0.5,-0.5,0.0,	0.0,1.0,0.0,		1.0,0.0,	//right bottom
	-0.5,0.5,0.0,	0.0,0.0,1.0,		0.0,1.0,	//left up
	0.5,0.5,0.0,		1.0,0.0,0.0,		1.0,1.0//right up
	};
	unsigned int idxs[] = {
		0,1,2,
		1,2,3
	};
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo_id;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxs), idxs, GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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
		process_input(w);

		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT);


		//unsigned int color_uni = glGetUniformLocation(shader_prog, "color");
		float time = glfwGetTime();
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

		//cout << time<<"\ttransp:"<<tranparency << endl;
		s.set_uniform_1f("tranparency", tranparency);

		mat4x4 trans = mat4x4(1.0f);
		
		trans = translate(trans, vec3(0.5, 0.5, 0));
		trans = scale(trans, vec3(0.5, 0.5, 0.5));
		trans = glm::rotate(trans, (float)time, vec3(0, 0, 1));
		
		s.set_transform_mat("transform", trans);
		glDrawElements(GL_TRIANGLES,sizeof(idxs),GL_UNSIGNED_INT,0);
		
		glfwSwapBuffers(w);
		glfwPollEvents();

	}
	s.delete_program();
	glDeleteBuffers(1, &vbo_id);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();

	return 0;
}