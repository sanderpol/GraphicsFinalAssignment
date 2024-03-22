#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "VAO.h"
#include "EBO.h"
#include "../CameraAndControls.h"
#include "Texture.h"


class Mesh
{
private:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	VAO thisVAO;

public:
	Mesh(VAO& vao, std::vector <Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	void Draw(
		Shader& shader,
		CameraAndControls* camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
};
#endif // !MESH_CLASS_H


