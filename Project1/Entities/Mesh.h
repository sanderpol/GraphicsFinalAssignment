#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "VAO.h"
#include "EBO.h"
#include "../CameraAndControls.h"
#include "Texture.h"


class Mesh
{
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	VAO VAO;

	Mesh(std::vector <Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	void Draw(Shader& shader, CameraAndControls& camera);
};
#endif // !MESH_CLASS_H


