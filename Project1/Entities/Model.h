#pragma once
#include <vector>
#include <iostream>
#include "../Libraries/glsl.h"
#include <glm/gtc/type_ptr.hpp>
#include "VAO.h"
#include "VBO.h"
#include <glm/glm.hpp>
struct SharedState
{
	std::string model_name;
	Vertex vertices;
};

struct UniqueState {
	VAO vao;
	glm::mat4 model;
	glm::vec3 ambient_color;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	float power;
};
class Model
{
private:
	SharedState* shared_state_;

public:
	Model(const SharedState* shared_state);
	Model(const Model& other);
	~Model();
	SharedState* shared_state() const;
	void InitBuffer(std::string model_name, GLuint& program_id, UniqueState& unique_state);
};

