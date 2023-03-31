#include "ModelFactory.h"

#pragma region privateFunctions

std::string ModelFactory::GetKey(const SharedState& ss) const {
	return ss.model_name;
}
#pragma endregion

ModelFactory::ModelFactory()
{
}

ModelFactory::ModelFactory(std::initializer_list<std::string> fileNames) {
	/*for (const string fileName : fileNames)
	{
		SharedState ss = CreateModelSharedState(fileName);
		this->models_.insert(std::make_pair(fileName, Model(&ss)));
	}*/
}

//SharedState ModelFactory::CreateModelSharedState(std::string fileName) {
//	vector<glm::vec3> normals;
//	vector<glm::vec3> vertices;
//	vector<glm::vec2> uvs;
//	bool res = loadOBJ("./Objects/teapot.obj", vertices, uvs, normals);
//	Vertex vertex {vertices, normals, uvs}
//	SharedState ss{ fileName, normals, vertices, uvs };
//	return ss;
//}

//Model ModelFactory::GetModel(const string key)
//{
//	if (this->models_.find(key) == this->models_.end()) {
//		//cant find model, so then it adds a new model to the flyweightFactory
//		SharedState ss = CreateModelSharedState(key);
//		this->models_.insert(std::make_pair(key, Model(&ss)));
//	}
//	
//	//return newly created model out of the flyweight factory
//	return this->models_.at(key);
//}
