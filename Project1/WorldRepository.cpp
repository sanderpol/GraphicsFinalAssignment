#include "WorldRepository.h"

ModelFactory WorldRepository::s_model_factory = ModelFactory();

void WorldRepository::InitializeWorldRepository()
{
	s_shader = Shader("./Shaders/vertexshader.vert", "./Shaders/fragmentshader.frag");

	std::initializer_list < std::string > list = {
		"teapot.obj"
	};
	



	//GLuint teapotVao = 10;
	//glm::mat4 model = glm::mat4(1.0f);
	//glm::vec3 ambient_color = glm::vec3(0.2, 0.2, 0.1);
	//glm::vec3 diffuse_color = glm::vec3(0.5, 0.5, 0.3);
	//glm::vec3 specular_color = glm::vec3(0.7, 0.7, 0.7);
	//float power = 1024;

	////GLuint torusVao = 11;
	////glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.5, 0.0, 0.0));
	////glm::vec3 ambient_color = glm::vec3(0.2, 0.2, 0.1);
	////glm::vec3 diffuse_color = glm::vec3(0.5, 0.5, 0.3);
	////glm::vec3 specular_color = glm::vec3(0.7, 0.7, 0.7);
	////float power = 1024;*/

	//UniqueState uniqueState{teapotVao, model, ambient_color, diffuse_color, specular_color, power };
	//s_world_models.insert(std::make_pair("teapot.obj", uniqueState));
	////Initialize 
	//s_model_factory = ModelFactory(list);

}

void WorldRepository::Render(CameraAndControls camara) {
		s_shader.Activate();		
		//std::string key = "teapot.obj";
		//Model teapot = s_model_factory.GetModel(key);
		//UniqueState teapotU = s_world_models.at("teapot.obj");

		///*auto it = s_world_models.find(key);
		//if (it != s_world_models.end()) {
		//	it->second.model = glm::rotate(teapotU.model, 0.01f, glm::vec3(0.5f, 1.0f, 0.2f));
		//	mv = view * it->second.model;
		//}*/
		////auto it = s_world_models.find(key);


		//// Send mv
		//glUniformMatrix4fv(s_uniform_mv, 1, GL_FALSE, glm::value_ptr(mv));

		//// Send vao
		//glBindVertexArray( teapotU.vao);
		//glDrawArrays(GL_TRIANGLES, 0, teapot.shared_state()->vertices.size());
		//glBindVertexArray(0);
	
}

void WorldRepository::InitBuffers(const glm::mat4 projection)
{
	//Model teapot = s_model_factory.GetModel("teapot.obj");
	//UniqueState teapotU = s_world_models.at("teapot.obj");

	//teapot.InitBuffer("teapot.obj", s_shader.ID, teapotU);

	//GLuint uniform_light_pos = glGetUniformLocation(s_shader.ID, "light_pos");
	//GLuint uniform_proj = glGetUniformLocation(s_shader.ID, "projection");
	//s_uniform_mv = glGetUniformLocation(s_shader.ID, "mv");

	//glUseProgram(s_shader.ID);

	////glUniformMatrix4fv(s_uniform_mv, 1, GL_FALSE, glm::value_ptr(mv));
	////glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(projection));
	//glUniform3fv(uniform_light_pos, 1, glm::value_ptr(glm::vec3(4.0, 4.0, 4.0)));
}
