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

extern vector<Texture> textures;

struct Vertex{
	vec3 position;
	vec3 normal;
	vec2 texcoords;
};

class Mesh {
public:
	Mesh(vector<Vertex>* verts, vector<unsigned int>* idxs, vector<unsigned int>* texidxs);
	Mesh(const Mesh& m);
	~Mesh();
	void draw(Shader& s);

	vector<Vertex> *vertices;
	vector<unsigned int>* indices;
	vector<unsigned int>* textureIndices;

public:
	unsigned int VAO, VBO, EBO;
	void setup_mesh();
};