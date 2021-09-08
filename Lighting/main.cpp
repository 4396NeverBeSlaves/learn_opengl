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
#include"light.h"
#include"stb_image.h"


using namespace std;
using namespace glm;

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

	glViewport(0, 0, WIDTH, HEIGHT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(w, framebuf_size_callback);
	glfwSetKeyCallback(w, key_callback);
	glfwSetScrollCallback(w, scroll_callback);
	glfwSetCursorPosCallback(w, cursor_callback);

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int lightvao;
	glGenVertexArrays(1, &lightvao);
	glBindVertexArray(lightvao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	Shader objshader("obj_shader.vert", "all_light_obj.frag");
	Shader lightshader("light_shader.vert", "light_shader.frag");

	Texture container2("container2.png", false);
	Texture container2_specular("container2_specular.png", false);
	Texture matrix("matrix.jpg", false);

	vec3 light_color(1.0, 1.0, 1.0);
	vec3 light_coef(1.0, 0.027, 0.0028);

	DirectionLight* dir_light = new DirectionLight("dir_light", vec3(1.0, 1.0, 1.0), vec3(0.0, -0.7, -0.7));
	PointLight* point_lights = new PointLight("point_lights", vec3(1.0, 1.0, 1.0), vec3(1.0, 1.5, 3.0), light_coef);
	SpotLight* spot_light = new SpotLight("spot_light", vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, -2.0), light_coef, vec3(0.0, 0.0, -1.0), 15.0, 16.0);

	float one_second = 0;
	int frame = 0;

	while (!glfwWindowShouldClose(w)) {
		delta_time = get_delta_time();
		one_second += delta_time;
		frame++;
		if (one_second >= 1.0) {
			string title = format("Lighting. [{:6.1f}FPS, {:5.2f}ms] [FOV: {:4.1f}] [Yaw:{:7.1f}, Pitch:{:5.1f}] [Position:{:5.1f} {:5.1f} {:5.1f}, Direction:{:4.1f} {:4.1f} {:4.1f}]",
				frame / one_second, one_second / frame * 1000, cam.fov, cam.yaw_, cam.pitch_, cam.cam_pos.x, cam.cam_pos.y, cam.cam_pos.z, cam.cam_dir.x, cam.cam_dir.y, cam.cam_dir.z);
			glfwSetWindowTitle(w, title.c_str());
			one_second = 0.0;
			frame = 0;
		}

		//cout << format("lag: {:4.2f}ms", delta_time * 1000) << endl;
		//light_color.x = sin(radians(current_time * 10))/2+0.5;
		//light_color.y = sin(radians(current_time * 10 + 120))/2+0.5;
		//light_color.z = sin(radians(current_time * 10 + 240))/2+0.5;

		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container2.get_texture_obj());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, container2_specular.get_texture_obj());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, matrix.get_texture_obj());



		mat4 view = cam.get_view_matrix();
		mat4 proj = perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
		mat4 model;

		{
			spot_light->position = spot_light->origin_postion;	//使用完更新后的灯光位置再重置为原位置
			spot_light->spot_dir = spot_light->origin_spot_dir;

			float rotate_radians = (float)radians(30.0) * current_time;
			model = mat4(1.0);
			mat4 dir_rotate_matrix = mat4(1.0);

			dir_rotate_matrix = rotate(dir_rotate_matrix, rotate_radians, vec3(0, 1, 0));

			model = translate(model, spot_light->position);
			model = rotate(model, rotate_radians, vec3(0, 1, 0));

			spot_light->position = vec3(model * vec4(0.0, 0.0, 0.0, 1.0));	//更新旋转后的灯光位置，使objshader能获得新的灯光位置
			spot_light->spot_dir = vec3(dir_rotate_matrix * vec4(spot_light->spot_dir, 0.0));

			model = scale(model, vec3(0.3f));
			lightshader.use();
			lightshader.set_matrix("model", model);
			lightshader.set_matrix("view", view);
			lightshader.set_matrix("projection", proj);
			lightshader.set_uniform_3fv("light_color", vec3(1.0, 1.0, 1.0));
			glBindVertexArray(lightvao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		for (int i = 0; i < 10; i++) {

			model = mat4(1.0);
			mat4 normal_matrix_translate(1.0);
			mat4 normal_matrix_rotate(1.0);

			model = translate(model, cubePositions[i]);
			normal_matrix_translate = { 1.0,0.0,0.0,-model[3][0],
										0.0,0.1,0.0,-model[3][1],
										0.0,0.0,1.0,-model[3][2],
										0.0,0.0,0.0,1.0
			};

			//model = rotate(model, (float)radians(10.0 * (i + 1)) * current_time, vec3(1, 0.3, 0.7));
			normal_matrix_rotate = { model[0][0],model[0][1],model[0][2],0.0,
									model[1][0],model[1][1],model[1][2],0.0,
									model[2][0],model[2][1],model[2][2],0.0,
									0.0,0.0,0.0,1.0

			};
			//mat4 t = normal_matrix_translate * normal_matrix_rotate;
			//mat4 normal_matrix = transpose(inverse(model));

			objshader.use();
			objshader.set_matrix("model", model);
			objshader.set_matrix("view", view);
			objshader.set_matrix("projection", proj);
			objshader.set_matrix("normal_matrix_translate", normal_matrix_translate);
			objshader.set_matrix("normal_matrix_rotate", normal_matrix_rotate);
			objshader.set_uniform_1f("time", current_time);
			objshader.set_uniform_3fv("eye_pos", cam.cam_pos);

			//dir_light->set_light(objshader);
			// point_lights->set_light(objshader);
			spot_light->set_light(objshader);
			//objshader.set_uniform_3fv("spot_light.position", vec3(cam.cam_pos.x + 0.5, cam.cam_pos.y - 0.5, cam.cam_pos.z));
			//objshader.set_uniform_3fv("spot_light.spot_dir", cam.cam_dir);
			//objshader.set_uniform_1f("light.inner_range_angle", 15.0);
			//objshader.set_uniform_1f("light.outer_range_angle", 16.0);
			//objshader.set_uniform_3fv("light.ambient", light_color * vec3(0.2, 0.2, 0.2));
			//objshader.set_uniform_3fv("light.diffuse", light_color * vec3(1.0, 1.0, 1.0));
			//objshader.set_uniform_3fv("light.specular", light_color * vec3(1.0, 1.0, 1.0));
			//objshader.set_uniform_1f("light.constant", light_coef[0]);
			//objshader.set_uniform_1f("light.linear", light_coef[1]);
			//objshader.set_uniform_1f("light.quadratic", light_coef[2]);

			objshader.set_texture("wood_box.diffuse", 0);
			objshader.set_texture("wood_box.specular", 1);
			objshader.set_texture("wood_box.emission", 2);
			objshader.set_uniform_1f("wood_box.shininess", 64);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	objshader.delete_program();
	lightshader.delete_program();
	glfwTerminate();
	return 0;
}