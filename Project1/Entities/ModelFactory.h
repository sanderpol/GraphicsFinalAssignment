#pragma once
#include <unordered_map>
#include <iostream>
#include "Model.h"
#include "../Libraries/objloader.h"
class ModelFactory
{
	

private:
	std::unordered_map<std::string, Model> models_;
	Model CreateModel(const std::string fileName);
public:
	ModelFactory();
	ModelFactory(std::initializer_list<std::string>);
	Model GetModel(const std::string key);	
};

