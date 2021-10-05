#include"Model.h"

Model::Model(const string& path, Shader* s) :shader(s), model_matrix(1.0) {
	load_model(path);
}

Model::~Model() {
	for (int i = 0; i < meshes.size(); i++)
		delete meshes[i];
}

void Model::draw() {

	shader->use();
	//shader->set_matrix("view", cam.get_view_matrix());
	//shader->set_matrix("projection", perspective((float)radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f));
	//shader->set_uniform_3fv("eye_pos", cam.cam_pos);
	shader->set_matrix("model", model_matrix);
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->draw(shader);
	}
	model_matrix = mat4(1.0);
}

void Model::translate(const vec3& v) {
	mat4 model_translate(1.0);
	model_translate = glm::translate(model_translate, v);
	model_matrix = model_matrix * model_translate;
}

void Model::rotate(float rad, const vec3& v) {
	mat4 model_rotate(1.0);
	model_rotate = glm::rotate(model_rotate, rad, v);
	model_matrix = model_matrix * model_rotate;
}

void Model::scale(const vec3& v) {
	mat4 model_scale(1.0);
	model_scale = glm::scale(model_scale, v);
	model_matrix = model_matrix * model_scale;
}

void Model::load_model(const string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "assimp scene error:" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of(R"('\')") + 1);

	process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene) {

	for (int i = 0; i < node->mNumMeshes; i++) {
		//cout << "will push in meshes!" << endl;
		//meshes.push_back(process_mesh(scene->mMeshes[node->mMeshes[i]], scene));
		process_mesh(scene->mMeshes[node->mMeshes[i]], scene);
		//cout << "in meshes!" << endl;
	}
	for (int j = 0; j < node->mNumChildren; j++) {
		process_node(node->mChildren[j], scene);
	}
}

void Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
	vector<Vertex>* vertices = new vector<Vertex>;
	vector<unsigned int>* indices = new vector<unsigned int>;
	vector<unsigned int>* textureIndices = new vector<unsigned int>;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vert;
		vert.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->HasNormals())
			vert.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0]) {
			vert.texcoords = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			vert.texcoords = vec2(0.0, 0.0);
		}
		vertices->push_back(vert);
		//vertices->emplace_back();
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices->push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		load_texture(material, textureIndices, aiTextureType_DIFFUSE, TextureType::Diffuse);
		load_texture(material, textureIndices, aiTextureType_SPECULAR, TextureType::Specular);
	}
	//cout << "making mesh! return mesh" << endl;
	meshes.push_back(new Mesh(vertices, indices, textureIndices));
	//meshes.emplace_back(vertices, indices, textureIndices);
	//meshes.push_back(Mesh(vertices, indices, textureIndices));
	//return Mesh(vertices, indices, textureIndices);
}

void Model::load_texture(aiMaterial* material, vector<unsigned int>* textureIndices, aiTextureType ai_tex_type, TextureType tex_type) {
	aiString tex_name;
	string tex_path;
	float shiness;

	for (int i = 0; i < material->GetTextureCount(ai_tex_type); i++) {

		material->GetTexture(ai_tex_type, i, &tex_name);
		tex_path = directory + string(tex_name.C_Str());

		int j;
		for (j = 0; j < textures.size(); j++) {
			if (textures[j].tex_path == tex_path)
				break;
		}

		material->Get(AI_MATKEY_SHININESS, shiness);
		//cout << "shininess: " << shiness << endl;
		if (j == textures.size())
			textures.push_back(Texture(tex_path, shiness, tex_type, false));

		textureIndices->push_back(j);

	}
}
