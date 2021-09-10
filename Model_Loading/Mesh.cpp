#include"Mesh.h"

void Mesh::draw(Shader& s) {
	int diffuse_counter = 1;
	int specular_counter = 1;

	for (int i = 0; i++; i < textures.size()) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D,textures[i].get_texture_obj());

		if (textures[i].type == TextureType::Diffuse) {
			s.set_texture(format("material.texture_diffuse{:d}", diffuse_counter),i);
			diffuse_counter++;
		}
		if (textures[i].type == TextureType::Specular) {
			s.set_texture(format("material.texture_specular{:d}", specular_counter), i);
			specular_counter++;
		}

	}
}

void  Mesh::setup_mesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texcoords));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}