#pragma once
#include <iostream>
#include "Libraries/glsl.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shaders/Shader.h"
#include "Entities/VAO.h"
#include "Entities/EBO.h"
#include "CameraAndControls.h"



class Skybox
{
private:
	Shader shader;
	VAO skybox_vao;
	GLuint cubemap_texture_id;
public:
	Skybox();
	void InitSkyboxShadersAndBuffers();
	void Render(CameraAndControls camera);
};

