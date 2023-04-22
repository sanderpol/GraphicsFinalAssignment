#include "ModelFactory.h"


ModelFactory::ModelFactory()
{
}

ModelFactory::ModelFactory(std::initializer_list<std::string> fileNames) {
	for (const std::string fileName : fileNames)
	{
		Model model = CreateModel(fileName);
		this->models_.insert(std::make_pair(fileName, model));
	}
}

Model ModelFactory::CreateModel(const std::string fileName) {
	Model model(fileName.c_str());
	return model;
}

Model ModelFactory::GetModel(const std::string key)
{
	if (this->models_.find(key) == this->models_.end()) {
		//cant find model, so then it adds a new model to the flyweightFactory
		Model model = CreateModel(key);
		this->models_.insert(std::make_pair(key, model));
	}

	//return newly created model out of the flyweight factory
	return this->models_.at(key);
}
