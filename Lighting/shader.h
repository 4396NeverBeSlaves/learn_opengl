#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace std;

class Shader {
public:
	unsigned int id;
	Shader(const string& vertex_shader_path,const string& frag_shader_path);

	void use();
	void delete_program();

	void set_uniform_3f(const string& name,float x,float y ,float z);
	void set_uniform_3fv(const string& name, glm::vec3 v);
	void set_uniform_4fv(const string& name, glm::vec4 v);
	void set_uniform_1f(const string& name, float x);
	void set_uniform_int(const string& name, int x, int y, int z);
	void set_uniform_bool(const string& name, bool x, bool y, bool z);
	void set_texture(const string& name,unsigned int texture);
	void set_matrix(const string& name, glm::mat4 mat);

private:
	void check_shader(int shader);
	void check_program(int shader_program);
};