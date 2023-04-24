#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include "../Libraries/glsl.h"
#include <vector>;

class EBO
{
public:
	GLuint ID;
	EBO(std::vector<GLuint>& Indices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif // !EBO_CLASS_H

