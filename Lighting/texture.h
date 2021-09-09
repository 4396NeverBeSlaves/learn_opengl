#pragma once
#include<iostream>
#include<format>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"../stb_image.h"

using namespace std;

class Texture {
public:
	Texture(const string& path,bool vertical_flip);
	unsigned int get_texture_obj();
public:
	unsigned int texture_id;
	int width, height,nchannels;
};