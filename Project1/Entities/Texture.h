#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H
#include "../Libraries/glsl.h"
#include "../Libraries/stb_image.h"
#include "../Shaders/Shader.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	static void Unbind();
	void Delete();
};
#endif // !TEXTURE_CLASS_H


