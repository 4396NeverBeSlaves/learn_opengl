#pragma once
#include"Model.h"

class ModelManger {
public:
	static void add_model(string path,Shader* s);
	static void add_model(Model* m);
	static void draw();
	static void destroy_all_models();
	static int get_models_num();
	static vector<Model*> models;
private:
	ModelManger() {}
};