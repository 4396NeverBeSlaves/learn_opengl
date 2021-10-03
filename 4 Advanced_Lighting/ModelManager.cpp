#include "ModelManager.h"

vector<Model*> ModelManger::models;

void ModelManger::add_model(string path, Shader* s) {
	models.push_back(new Model(path,s));
}

void ModelManger::add_model(Model* m) {
	models.push_back(m);
}

void ModelManger::draw() {
	for (vector<Model*>::iterator it = models.begin(); it != models.end(); it++) {
		(*it)->draw();
	}
}

void ModelManger::destroy_all_models() {
	for (vector<Model*>::iterator it=models.begin(); it != models.end();it++) {
		delete (*it);
	}
}

int ModelManger::get_models_num() {
	return models.size();
}
