#include "WorldRepository.h"
//#include "Libraries/boost/filesystem.hpp"
//namespace fs = boost::filesystem;

ModelFactory WorldRepository::s_model_factory = ModelFactory();

WorldRepository::WorldRepository()
{
	s_shader = Shader("./Shaders/vertexshader.vert", "./Shaders/fragmentshader.frag");
	s_shader.Activate();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	
	glUniform4f(glGetUniformLocation(s_shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(s_shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	//std::string parentDir = 
	//s_model_factory.GetModel("./Resources/models/bunny/scene.gltf");
	s_shader.Activate();
}

//WorldRepository::~WorldRepository()
//{
//	s_shader.Delete();
//}

void WorldRepository::Render(CameraAndControls* camara) {
	
	s_shader.Activate();
	Model model = s_model_factory.GetModel("./Resources/models/bunny/scene.gltf");
	model.Draw(s_shader, camara);	
}
