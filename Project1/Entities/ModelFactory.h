#pragma once
#include <unordered_map>
#include <iostream>
#include "Model.h"
#include "../Libraries/objloader.h"
class ModelFactory
{
private:
	std::unordered_map<std::string, Model> models_;
	std::string GetKey(const SharedState& ss) const;
	SharedState CreateModelSharedState(std::string fileName);
public:
	ModelFactory();
	ModelFactory(std::initializer_list<std::string>);
	Model GetModel(std::string key);	
};

