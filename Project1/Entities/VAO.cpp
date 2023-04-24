#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &this->ID);
}

void VAO::LinkVBO(VBO vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	vbo.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(this->ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &this->ID);
}