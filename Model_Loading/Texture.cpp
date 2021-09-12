#include"Texture.h"

Texture::Texture(const string& path, float shiness, TextureType tp, bool vertical_flip):specular_shininess(shiness),type(tp),tex_path(path) {
	stbi_set_flip_vertically_on_load(vertical_flip);

	unsigned char* tex_file = stbi_load(path.c_str(), &width, &height, &nchannels, 0);
	if (!tex_file) 
		cerr << path<<" not found!" << endl;
	
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(nchannels==4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_file);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_file);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	cout <<"Texture loaded: "<< path << endl;
	stbi_image_free(tex_file);
}

unsigned int Texture::get_texture_obj() {
	return texture_id;
}