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
	Texture(const string& path, TextureType tp,bool vertical_flip);
	unsigned int get_texture_obj();
	const TextureType type;
public:
	unsigned int texture_id;
	int width, height,nchannels;
};