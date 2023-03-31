#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include "../Libraries/glsl.h"
#include "VBO.h"



class VAO
{
public:
	GLuint ID;
	VAO();
	void LinkVBO(VBO vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	static void Unbind();
	void Delete();
};

#endif // !VAO_H
