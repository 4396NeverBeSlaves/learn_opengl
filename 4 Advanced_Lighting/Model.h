#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<glm/glm.hpp>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"Camera.h"
#include"Texture.h"
#include"Shader.h"
#include"Mesh.h"

using namespace std;

extern  vector<Texture> textures;
extern Camera cam;
extern const int WIDTH;
extern const int HEIGHT;

class Model {
public:
	Model(const string& path,Shader* s);
	~Model();
	void draw();
	void translate(const vec3& v);
	void rotate(float rad, const vec3& v);
	void scale(const vec3& v);

	Shader* shader;
	string directory;
private:
	void load_model(const string& path);
	void process_node(aiNode* node, const aiScene* scene);
	void process_mesh(aiMesh* mesh, const aiScene* scene);
	void load_texture(aiMaterial *material, vector<unsigned int>* textureIndices, aiTextureType ai_tex_type,TextureType tex_type);

	vector<Mesh*> meshes;
	mat4 model_matrix;
};