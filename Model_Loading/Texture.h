#pragma once
#include<iostream>
#include<format>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"../stb_image.h"

using namespace std;



enum class TextureType {
	Diffuse,
	Specular
};

class Texture {
public:
	Texture(const string& path,float shiness, TextureType tp,bool vertical_flip);
	unsigned int get_texture_obj();

	
	unsigned int texture_id;
	int width, height,nchannels;
	float specular_shininess;
	const TextureType type;
	const string tex_path;
};