#include"shader.h"

Shader::Shader(const string& vertex_shader_path, const string& frag_shader_path){
	fstream vs_file, fs_file;
	stringstream vs_string_stream, fs_string_stream;
	string vertex_shader_source, frag_shader_source;

	vs_file.exceptions(fstream::badbit | fstream::failbit);
	fs_file.exceptions(fstream::badbit | fstream::failbit);
	try {
		vs_file.open(vertex_shader_path);
		fs_file.open(frag_shader_path);

		vs_string_stream<<vs_file.rdbuf();
		fs_string_stream << fs_file.rdbuf();

		vertex_shader_source = vs_string_stream.str();
		frag_shader_source = fs_string_stream.str();

		vs_file.close();
		fs_file.close();
	}
	catch (fstream::failure e) {
		cout << "Read shader file failed!" << endl;
	}

	const char* vs_source = vertex_shader_source.c_str();
	const char* fs_source = frag_shader_source.c_str();


	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	id = glCreateProgram();

	glShaderSource(vertex_shader, 1, &vs_source, NULL);
	glCompileShader(vertex_shader);
	check_shader(vertex_shader);

	glShaderSource(frag_shader, 1, &fs_source, NULL);
	glCompileShader(frag_shader);
	check_shader(frag_shader);

	glAttachShader(id, vertex_shader);
	glAttachShader(id, frag_shader);
	glLinkProgram(id);
	check_program(id);
	glDeleteShader(vertex_shader);
	glDeleteShader(frag_shader);

}

void Shader::check_shader(int shader) {
	int compile_status;
	char log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status) {
		glGetShaderInfoLog(shader, 512, NULL, log);
		cout << "vertex shader compile failed:" << log << endl;
	}
}

void Shader::check_program(int shader_program) {
	int status;
	char log[512];

	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	if (status == false) {
		glGetProgramInfoLog(shader_program, 512, NULL, log);
	}
}
void Shader::use() {
	glUseProgram(id);
}

void Shader::delete_program() {
	glDeleteProgram(id);
}

void Shader::set_uniform_3f(const string& name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
void Shader::set_uniform_1f(const string& name, float x) {
	glUniform1f(glGetUniformLocation(id, name.c_str()), x);
}
void Shader::set_uniform_int(const string& name, int x, int y, int z) {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
void Shader::set_uniform_bool(const string& name, bool x, bool y, bool z) {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::set_texture(const string& name, unsigned int texture) {
	glUniform1i(glGetUniformLocation(id,name.c_str()),texture);
}

void Shader::set_transform_mat(const string& name, glm::mat4 trans) {
	glUniformMatrix4fv(glGetUniformLocation(id,name.c_str()),1,false,glm::value_ptr(trans));
}