#include<iostream>
#include<format>
#include<vector>
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
	glDepthFunc(GL_LESS);
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(w, framebuf_size_callback);
	glfwSetKeyCallback(w, key_callback);
	glfwSetScrollCallback(w, scroll_callback);
	glfwSetCursorPosCallback(w, cursor_callback);
	float cubeVertices[] = {
		// positions          // texture Coords  //normal
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f
	};
	float planeVertices[] = {
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
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

	unsigned int cubemapVAO, cubemapVBO;
	glGenVertexArrays(1, &cubemapVAO);
	glGenBuffers(1, &cubemapVBO);
	glBindVertexArray(cubemapVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	const int x_segments = 50;
	const int y_segments = 50;

	vector<float> sphere_vertices;
	vector<unsigned int> sphere_idxs;
	for (size_t i = 0; i <= y_segments; i++) {
		for (size_t j = 0; j <= x_segments; j++) {
			float x_segment = j * 1.0 / x_segments;
			float y_segment = i * 1.0 / y_segments;

			float x = sin(pi<float>() * y_segment) * cos((2 * pi<float>() * x_segment));
			float y = cos(pi<float>() * y_segment);
			float z = sin(pi<float>() * y_segment) * sin((2 * pi<float>() * x_segment));
			sphere_vertices.push_back(x);
			sphere_vertices.push_back(y);
			sphere_vertices.push_back(z);
		}
	}

	for (size_t i = 0; i < y_segments; i++) {	//两个逆时针顺序的三角形
		for (size_t j = 0; j < x_segments; j++) {
			sphere_idxs.push_back(i * (x_segments + 1) + j);
			sphere_idxs.push_back(i * (x_segments + 1) + j + 1);
			sphere_idxs.push_back((i + 1) * (x_segments + 1) + j + 1);
			sphere_idxs.push_back(i * (x_segments + 1) + j);
			sphere_idxs.push_back((i + 1) * (x_segments + 1) + j + 1);
			sphere_idxs.push_back((i + 1) * (x_segments + 1) + j);
		}
	}

	unsigned int sphereVAO, sphereVBO, sphereEBO;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere_vertices.size() * sizeof(float), sphere_vertices.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &sphereEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_idxs.size() * sizeof(int), sphere_idxs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);


	string cubemap_path[] = { R"(./skybox/right.jpg)",R"(./skybox/left.jpg)" ,R"(./skybox/top.jpg)",
							R"(./skybox/bottom.jpg)" ,R"(./skybox/front.jpg)" ,R"(./skybox/back.jpg)" };
	Texture cube_texture("marble.jpg", true);
	Texture plane_texture("metal.png", true);
	Texture cubemap(cubemap_path, false);

	Shader obj_shader("geometry_shader08_explosion_normal_obj_shader.vert", "geometry_shader08_explosion_normal_obj_shader.frag");
	Shader obj_normal_shader("geometry_shader08_explosion_normal_obj_shader.vert","geometry_shader08_explosion_normal_obj_shader.geom", "geometry_shader08_explosion_normal_obj_shader_normal.frag");
	Shader cubemap_shader("geometry_shader08_explosion_normal_cubemap_shader.vert", "geometry_shader08_explosion_normal_cubemap_shader.frag");
	Shader sphere_shader("geometry_shader08_explosion_normal_sphere.vert", "geometry_shader08_explosion_normal_sphere.geom","geometry_shader08_explosion_normal_sphere.frag");


	float one_second = 0;
	int frame = 0;
	while (!glfwWindowShouldClose(w)) {
		delta_time = get_delta_time();
		one_second += delta_time;
		frame++;
		if (one_second >= 1.0) {
			string title = format("Cubemap. [{:6.1f}FPS, {:5.2f}ms] [FOV: {:4.1f}] [Yaw:{:7.1f}, Pitch:{:5.1f}] [Position:{:5.1f} {:5.1f} {:5.1f}, Direction:{:4.1f} {:4.1f} {:4.1f}]",
				frame / one_second, one_second / frame * 1000, cam.fov, cam.yaw_, cam.pitch_, cam.cam_pos.x, cam.cam_pos.y, cam.cam_pos.z, cam.cam_dir.x, cam.cam_dir.y, cam.cam_dir.z);
			glfwSetWindowTitle(w, title.c_str());
			one_second = 0.0;
			frame = 0;
		}

		glClearColor(0.0, 0.0, 0.0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		mat4 view = cam.get_view_matrix();
		mat4 proj = perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
		mat4 model = mat4(1.0);

		obj_shader.use();
		obj_shader.set_matrix("view", view);
		obj_shader.set_matrix("projection", proj);
		obj_shader.set_matrix("model", translate(mat4(1.0), vec3(-2.0, 0.0, 1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture.get_texture_obj());
		obj_shader.set_texture("diffusemap", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.get_texture_obj());
		obj_shader.set_texture("cubemap", 1);
		obj_shader.set_uniform_3fv("eye_pos", cam.cam_pos);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / 5);
		glBindVertexArray(0);

		obj_normal_shader.use();
		obj_normal_shader.set_matrix("view", view);
		obj_normal_shader.set_matrix("projection", proj);
		obj_normal_shader.set_matrix("model", translate(mat4(1.0), vec3(-2.0, 0.0, 1.0)));
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / 5);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture.get_texture_obj());
		obj_shader.use();
		obj_shader.set_matrix("model", translate(mat4(1.0), vec3(-2.8, 0, 2.5)));
		obj_shader.set_texture("diffusemap", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.get_texture_obj());
		obj_shader.set_texture("cubemap", 1);
		obj_shader.set_uniform_3fv("eye_pos", cam.cam_pos);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / 5);
		glBindVertexArray(0);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plane_texture.get_texture_obj());
		glBindVertexArray(planeVAO);
		obj_shader.use();
		obj_shader.set_matrix("model", model);
		obj_shader.set_texture("diffusemap", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.get_texture_obj());
		obj_shader.set_texture("cubemap", 1);
		obj_shader.set_uniform_3fv("eye_pos", cam.cam_pos);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(planeVertices) / 5);
		glBindVertexArray(0);

		sphere_shader.use();
		sphere_shader.set_matrix("model", model);
		sphere_shader.set_matrix("view", view);
		sphere_shader.set_matrix("projection", proj);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.get_texture_obj());
		sphere_shader.set_texture("cubemap", 0);
		sphere_shader.set_uniform_3fv("eye_pos", cam.cam_pos);
		sphere_shader.set_uniform_1f("time", current_time);
		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLES, x_segments * y_segments * 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		glDepthFunc(GL_LEQUAL);
		cubemap_shader.use();
		cubemap_shader.set_matrix("view", mat4(mat3(view)));
		cubemap_shader.set_matrix("projection", proj);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.get_texture_obj());
		cubemap_shader.set_texture("cubemap", 0);
		glBindVertexArray(cubemapVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	//glDeleteBuffers(1, &vbo);
	//glDeleteVertexArrays(1, &vao);
	obj_shader.delete_program();
	cubemap_shader.delete_program();
	glfwTerminate();
	return 0;
}
