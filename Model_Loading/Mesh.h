#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<glm/glm.hpp>
#include"Texture.h"
#include"Shader.h"

using namespace std;
using namespace glm;

struct Vertex{
	vec3 position;
	vec3 normal;
	vec3 texcoords;
};

class Mesh {
public:
	Mesh(const vector<Vertex>& verts, const vector<unsigned int>& idxs, const vector<Texture>& texs):
	vertices(verts),indices(idxs),textures(texs){
		setup_mesh();
	}
	void draw(Shader& s);

	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

private:
	unsigned int VAO, VBO, EBO;
	void setup_mesh();
};