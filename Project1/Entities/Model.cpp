#include "Model.h"

Model::Model(const char* file)
{
	// Make a JSON object
	std::string text = getFileContents(file);
	JSON = json::parse(text);

	// Get the binary data
	Model::file = file;
	data = getData();

	// Traverse all nodes
	traverseNode(0);
}

void Model::Draw(Shader& shader, CameraAndControls* camera)
{
	// Go over all meshes and draw each one
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
	}
}

void Model::loadMesh(unsigned int indMesh)
{
	// Get all accessor indices
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	// Use accessor indices to get all vertices components
	std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
	std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
	std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

	// Combine all the vertex components and also get the indices and textures
	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
	std::vector<Texture> textures = getTextures();

	VAO vao = VAO();
	Mesh mesh(vao, vertices, indices, textures);
	// Combine the vertices, indices, and textures into a mesh
	meshes.push_back(mesh);
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	// Current node
	json node = JSON["nodes"][nextNode];

	// Get translation if it exists
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = glm::make_vec3(transValues);
	}
	// Get quaternion if it exists
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}
	// Get scale if it exists
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}
	// Get matrix if it exists
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Multiply all matrices together
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	// Check if the node contains a mesh and if it does load it
	if (node.find("mesh") != node.end())
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	// Check if the node has children, and if it does, apply this function to them with the matNextNode
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

std::vector<unsigned char> Model::getData()
{
	// Create a place to store the raw text, and get the uri of the .bin file
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	// Store raw text data into bytesText
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = getFileContents((fileDirectory + uri).c_str());

	// Transform the raw text data into bytes and put them in a vector
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> Model::getFloats(json accessor)
{
	std::vector<float> floatVec;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Interpret the type and store it into numPerVert
	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	// Go over all the bytes in the data at the correct place using the properties from above
	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
	std::vector<GLuint> indices;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Get indices with regards to their type: unsigned int, unsigned short, or short
	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<Texture> Model::getTextures()
{
	std::vector<Texture> textures;

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Go over all images
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		// uri of current texture
		std::string texPath = JSON["images"][i]["uri"];

		// Check if the texture has already been loaded
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexNames.size(); j++)
		{
			if (loadedTexNames[j] == texPath)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		// If the texture has been loaded, skip this
		if (!skip)
		{
			// Load diffuse texture
			if (texPath.find("baseColor") != std::string::npos)
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTextures.size());
				textures.push_back(diffuse);
				loadedTextures.push_back(diffuse);
				loadedTexNames.push_back(texPath);
			}
			// Load specular texture
			else if (texPath.find("metallicRoughness") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTextures.size());
				textures.push_back(specular);
				loadedTextures.push_back(specular);
				loadedTexNames.push_back(texPath);
			}
		}
	}

	return textures;
}

std::vector<Vertex> Model::assembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i]
			}
		);
	}
	return vertices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	return vectors;
}
std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}
std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}
//#include "Model.h"
//
//Model::Model(const char* fileName)
//{
//	std::string text = getFileContents(fileName);
//	JSON = json::parse(text);
//
//	Model::file = fileName;
//	data = getData();
//
//	traverseNodes(0);
//}
//
//void Model::Draw(Shader& shader, CameraAndControls& camera)
//{
//	for (unsigned int i = 0; i < meshes.size(); i++) {
//		meshes[i].Draw(shader, camera, matricesMeshes[i], translationsMeshes[i], rotationsMeshes[i], scalesMeshes[i]);
//	}
//}
//
//void Model::loadMesh(unsigned int indMesh)
//{
//	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
//	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
//	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
//	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];
//
//	std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
//	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
//
//	std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
//	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
//
//	std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
//	std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);
//
//	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
//	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
//	std::vector<Texture> textures = GetTextures();
//
//	meshes.push_back(Mesh(vertices, indices, textures));
//}
//
//void Model::traverseNodes(unsigned int nextNode, glm::mat4 matrix)
//{
//	json node = JSON["nodes"][nextNode];
//
//	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
//	if (node.find("translation") != node.end()) {
//		float transValues[3];
//		for (unsigned int i = 0; i < node["translation"].size(); i++) {
//			transValues[i] = (node["translation"][i]);
//		}
//		translation = glm::make_vec3(transValues);
//	}
//
//	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
//	if (node.find("rotation") != node.end()) {
//		//gltf uses x,y,z,w format, while glm uses w,x,y,z format
//		float rotValues[4] = {
//			node["rotation"][3],
//			node["rotation"][0],
//			node["rotation"][1],
//			node["rotation"][2]
//		};
//		rotation = glm::make_quat(rotValues);
//	}
//
//	glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
//	if (node.find("scale") != node.end()) {
//		float scaleValues[3];
//		for (unsigned int i = 0; i < node["scale"].size(); i++) {
//			scaleValues[i] = (node["scale"][i]);
//		}
//		scale = glm::make_vec3(scaleValues);
//	}
//
//	glm::mat4 matNode = glm::mat4(1.0f);
//	if (node.find("matrix") != node.end()) {
//		float matValues[16];
//		for (unsigned int i = 0; i < node["matrix"].size(); i++) {
//			matValues[i] = (node["matrix"][i]);
//		}
//		matNode = glm::make_mat4(matValues);
//	}
//
//
//	glm::mat4 trans = glm::mat4(1.0f);
//	glm::mat4 rot = glm::mat4(1.0f);
//	glm::mat4 sca = glm::mat4(1.0f);
//
//	trans = glm::translate(trans, translation);
//	rot = glm::mat4_cast(rotation);
//	sca = glm::scale(sca, scale);
//
//	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;
//
//	if (node.find("mesh") != node.end()) {
//		translationsMeshes.push_back(translation);
//		rotationsMeshes.push_back(rotation);
//		scalesMeshes.push_back(scale);
//		matricesMeshes.push_back(matNextNode);
//
//		loadMesh(node["mesh"]);
//	}
//
//	if (node.find("children") != node.end()) {
//		for (unsigned int i = 0; i < node["children"].size(); i++) {
//			traverseNodes(node["children"][i], matNextNode);
//		}
//	}
//
//
//}
//
//std::vector<unsigned char> Model::getData()
//{
//	std::string bytesText;
//	std::string uri = JSON["buffers"][0]["uri"];
//
//	std::string fileStr = std::string(file);
//	std::string fileDir = fileStr.substr(0, fileStr.find_last_of('/') + 1);
//	bytesText = getFileContents((fileDir + uri).c_str());
//
//	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
//	return data;
//}
//
//std::vector<float> Model::getFloats(json accessor)
//{
//	std::vector<float> floatVec;
//
//	unsigned int buffViewInd = accessor.value("bufferView", 1);
//	unsigned int count = accessor["count"];
//	unsigned int accByteOffset = accessor.value("byteOffset", 0);
//	std::string type = accessor["type"];
//
//	json bufferView = JSON["bufferViews"][buffViewInd];
//	unsigned int byteOffset = bufferView["byteOffset"];
//
//	unsigned int numPerVert;
//	if (type == "SCALER") numPerVert = 1;
//	else if (type == "VEC2") numPerVert = 2;
//	else if (type == "VEC3") numPerVert = 3;
//	else if (type == "VEC4") numPerVert = 4;
//	else throw std::invalid_argument("Type is invalid (Use SCALER, VEC2, VEC3 OR VEC4)");
//
//	unsigned int beginningOfData = byteOffset + accByteOffset;
//	unsigned int lengthOfData = count * 4 * numPerVert;
//	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i) {
//		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
//		float value;
//		std::memcpy(&value, bytes, sizeof(float));
//		floatVec.push_back(value);
//	}
//
//	return floatVec;
//}
//
//std::vector<GLuint> Model::getIndices(json accessor)
//{
//	std::vector<GLuint> indices;
//
//	unsigned int buffViewInd = accessor.value("bufferView", 1);
//	unsigned int count = accessor["count"];
//	unsigned int accByteOffset = accessor.value("byteOffset", 0);
//	unsigned int componentType = accessor["type"];
//
//	json bufferView = JSON["bufferViews"][buffViewInd];
//	unsigned int byteOffset = bufferView["byteOffset"];
//
//	unsigned int beginningOfData = byteOffset + accByteOffset;
//	if (componentType == 5125) {
//
//		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i) {
//			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
//			float value;
//			std::memcpy(&value, bytes, sizeof(unsigned int));
//			indices.push_back((GLuint)value);
//		}
//	}
//	else if (componentType == 5123) {
//		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i) {
//			unsigned char bytes[] = { data[i++], data[i++] };
//			unsigned short value;
//			std::memcpy(&value, bytes, sizeof(unsigned short));
//			indices.push_back((GLuint)value);
//		}
//	}
//	else if (componentType == 5122) {
//		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i) {
//			unsigned char bytes[] = { data[i++], data[i++] };
//			short value;
//			std::memcpy(&value, bytes, sizeof(short));
//			indices.push_back((GLuint)value);
//		}
//	}
//	return indices;
//}
//
//std::vector<Texture> Model::GetTextures()
//{
//	std::vector<Texture> textures;
//
//	std::string fileStr = std::string(file);
//	std::string fileDir = fileStr.substr(0, fileStr.find_last_of('/') + 1);
//
//	unsigned int unit = 0;
//	for (unsigned int i = 0; i < JSON["images"].size(); i++) {
//		std::string texPath = JSON["images"][i]["uri"];
//
//		bool skip = false;
//		for (unsigned int j = 0; j < loadedTexNames.size(); j++) {
//			if (loadedTexNames[j] == texPath) {
//				textures.push_back(loadedTextures[j]);
//				skip = true;
//				break;
//			}
//		}
//		if (!skip) {
//			if (texPath.find("baseColor") != std::string::npos) {
//				Texture diffuse = Texture((fileDir + texPath).c_str(), "diffuse", loadedTextures.size());
//				textures.push_back(diffuse);
//				loadedTextures.push_back(diffuse);
//				loadedTexNames.push_back(texPath);
//			}
//			else if (texPath.find("matallicRoughness") != std::string::npos) {
//				Texture specular = Texture((fileDir + texPath).c_str(), "specular", loadedTextures.size());
//				textures.push_back(specular);
//				loadedTextures.push_back(specular);
//				loadedTexNames.push_back(texPath);
//			}
//		}
//	}
//	return textures;
//}
//
//std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs)
//{
//	std::vector<Vertex> vertices;
//	for (unsigned int i = 0; i < positions.size(); i++) {
//		vertices.push_back(
//			Vertex{
//				positions[i],
//				normals[i],
//				//Colors are not taken in to account when using 3D models, so an simple white vector is suffcient;
//				glm::vec3(1.0f, 1.0f, 1.0f),
//				texUVs[i]
//			}
//		);
//	}
//	return vertices;
//}
//
//std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
//{
//	std::vector<glm::vec2> vectors;
//	for (int i = 0; i < floatVec.size(); i) {
//		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
//	}
//	return vectors;
//}
//
//std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
//{
//	std::vector<glm::vec3> vectors;
//	for (int i = 0; i < floatVec.size(); i) {
//		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
//	}
//	return vectors;
//}
//
//std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
//{
//	std::vector<glm::vec4> vectors;
//	for (int i = 0; i < floatVec.size(); i) {
//		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
//	}
//	return vectors;
//}
//
