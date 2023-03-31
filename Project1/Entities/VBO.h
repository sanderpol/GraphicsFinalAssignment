#ifndef VBO_CLASS_H
#define VBO_CLASS_H
#include "../Libraries/glsl.h";
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 colors;
	glm::vec2 uvs;
};
struct VertexSkybox {
	glm::vec3 position;
};
class VBO
{
public: 
	GLuint ID;
	VBO(std::vector<Vertex>& vertices);
	VBO(std::vector<VertexSkybox>& vertices);
	void Bind();
	static void Unbind();
	void Delete();
};
#endif // !VBO_H


