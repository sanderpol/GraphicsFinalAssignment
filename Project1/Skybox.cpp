#include "Skybox.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/stb_image.h"



VertexSkybox skyboxVertices[] =
{
	//   Coordinates
	VertexSkybox{glm::vec3(-1.0f, -1.0f,  1.0f)},//        7--------6
	VertexSkybox{glm::vec3(1.0f, -1.0f,  1.0f)},//       /|       /|
	VertexSkybox{glm::vec3(1.0f, -1.0f, -1.0f)},//      4--------5 |
	VertexSkybox{glm::vec3(-1.0f, -1.0f, -1.0f)},//      | |      | |
	VertexSkybox{glm::vec3(-1.0f,  1.0f,  1.0f)},//      | 3------|-2
	VertexSkybox{glm::vec3(1.0f,  1.0f,  1.0f)},//      |/       |/
	VertexSkybox{glm::vec3(1.0f,  1.0f, -1.0f)},//      0--------1
	VertexSkybox{glm::vec3(-1.0f,  1.0f, -1.0f)}
};

GLuint skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};


Skybox::Skybox()
{

}

void Skybox::InitSkyboxShadersAndBuffers() {
	shader = Shader("./Shaders/vertexshader_skybox.vert", "./Shaders/fragmentshader_skybox.frag");
	skybox_vao = VAO();
	skybox_vao.Bind();

	std::vector <VertexSkybox> verts(skyboxVertices, skyboxVertices + sizeof(skyboxVertices) / sizeof(VertexSkybox));
	std::vector <GLuint> ind(skyboxIndices, skyboxIndices + sizeof(skyboxIndices) / sizeof(GLuint));
	VBO vbo = VBO(verts);
	EBO ebo = EBO(ind);
	skybox_vao.LinkVBO(vbo, 0, 3, GL_FLOAT, sizeof(VertexSkybox), (void*)0);

	VAO::Unbind();
	VBO::Unbind();
	EBO::Unbind();


	std::string facesCubemap[6] =
	{
		"./Resources/skybox/posx.jpg",
		"./Resources/skybox/negx.jpg",
		"./Resources/skybox/posy.jpg",
		"./Resources/skybox/negy.jpg",
		"./Resources/skybox/posz.jpg",
		"./Resources/skybox/posy.jpg"
	};

	// Creates the cubemap texture object
	glGenTextures(1, &cubemap_texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture_id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}

	GLuint skybox_uv = glGetUniformLocation(shader.ID, "skybox");
	glUniform1i(skybox_uv, 0);
	
}


void Skybox::Render(CameraAndControls camera) {

	shader.Activate();
	// Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
	glDepthFunc(GL_LEQUAL);

	// the viewmatrix is put through the constructor of the glm::mat3 to delete the last column containing
	// the translation paramaters, witch are not necessery
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 projection = camera.GetProjectionMatrix();

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
	// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
	// where an object is present (a depth of 1.0f will always fail against any object's depth value)
	glBindVertexArray(skybox_vao.ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture_id);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Switch back to the normal depth function
	glDepthFunc(GL_LESS);
}