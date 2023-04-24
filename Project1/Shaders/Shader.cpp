#include "Shader.h"
std::string getFileContents(const char* fileName)
{
	std::ifstream in(fileName, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw (errno);
}
Shader::Shader() {

}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	char* vertexshader = glsl::readFile(vertexFile);
	GLuint vsh_id = glsl::makeVertexShader(vertexshader);

	compileErrors(vsh_id, "VERTEX");

	char* fragshader = glsl::readFile(fragmentFile);
	GLuint fsh_id = glsl::makeFragmentShader(fragshader);

	compileErrors(fsh_id, "FRAGMENT");

	this->ID = glsl::makeShaderProgram(vsh_id, fsh_id);

	compileErrors(ID, "PROGRAM");
}

void Shader::Activate() {
	glUseProgram(this->ID);
}

void Shader::Delete()
{
	glDeleteProgram(this->ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}