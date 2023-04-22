#include "WorldRepository.h"

ModelFactory WorldRepository::s_model_factory = ModelFactory();

void WorldRepository::InitializeWorldRepository()
{
	s_shader = Shader("./Shaders/vertexshader.vert", "./Shaders/fragmentshader.frag");
		
	s_model_factory.GetModel("./Resources/models/sword/scene.gltf");

}

void WorldRepository::Render(CameraAndControls camara) {
		s_shader.Activate();		

		Model model = s_model_factory.GetModel("./Resources/models/sword/scene.gltf");
		model.Draw(s_shader, camara);	
}
