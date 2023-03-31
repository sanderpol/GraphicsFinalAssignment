#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <iostream>
#include <sstream>
#include <fstream>

#include "../Libraries/glsl.h"

std::string getFileContents(const char* fileName);

class Shader
{
public:
	GLuint ID;
	Shader();
	Shader(const char* vertexFile, const char* fragmentFile);
	void Activate();
	void Delete();
private:
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);
};


#endif // !SHADER_H
