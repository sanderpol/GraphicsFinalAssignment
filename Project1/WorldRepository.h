#pragma once
#include <iostream>
#include <unordered_map>
#include "Entities/ModelFactory.h"
#include "Entities/Model.h";
#include "Shaders/Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CameraAndControls.h"




class WorldRepository
{
private:
	static ModelFactory s_model_factory;
	Shader s_shader;
	
public:
	WorldRepository();
	~WorldRepository();
	void Render(CameraAndControls camara);
	
};

