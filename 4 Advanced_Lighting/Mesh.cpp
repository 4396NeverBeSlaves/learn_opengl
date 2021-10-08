#include"Mesh.h"

Mesh::Mesh(vector<Vertex> *verts, vector<unsigned int>* idxs, vector<unsigned int>* texidxs) :
	vertices(verts), indices(idxs), textureIndices(texidxs) {
	//cout << "Mesh::Mesh() VAO: "<<&VAO << endl;
	setup_mesh();
}

Mesh::Mesh(const Mesh& m) {
	this->VAO = m.VAO;
	this->VBO = m.VBO;
	this->EBO = m.EBO;
	this->vertices = m.vertices;
	this->indices = m.indices;
	this->textureIndices = m.textureIndices;
	//cout << "Mesh::Mesh(const Mesh& m) oldVAO: "<<&m.VAO<<" newVAO: "<<&VAO << endl;
}

Mesh::~Mesh() {
	//cout <<"Mesh::~Mesh() VAO: "<<&VAO << endl;
	delete vertices;
	delete indices;
	delete textureIndices;
	//glDeleteBuffers(1, &EBO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw(Shader* s) {
	int diffuse_counter = 0;
	int specular_counter = 0;

	s->use();
	for (int i = 0; i < textureIndices->size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D,textures[(*textureIndices)[i]].get_texture_obj());
		
		if (textures[(*textureIndices)[i]].type == TextureType::Diffuse) {
			s->set_texture(format("material.texture_diffuse{:d}", diffuse_counter),i);
			diffuse_counter++;
		}
		if (textures[(*textureIndices)[i]].type == TextureType::Specular) {
			s->set_uniform_1f("material.shininess", textures[(*textureIndices)[i]].specular_shininess);
			s->set_texture(format("material.texture_specular{:d}", specular_counter), i);
			specular_counter++;
		}
		
	}
	//cout <<"Mesh::draw  all count: "<<	textureIndices->size()<<" diff count: " << diffuse_counter  << " spec count: " << specular_counter << endl;
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
	for (int i = 0; i < textureIndices->size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	s->set_uniform_1f("material.shininess", 0);
	glBindVertexArray(0);
}

void  Mesh::setup_mesh() {
	//cout << "vao vbo ebo " << VAO << VBO << EBO << endl;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//cout << "after vao vbo ebo " << VAO << VBO << EBO << endl;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), vertices->data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texcoords));
	glEnableVertexAttribArray(2);


	glBindVertexArray(0);
}