#ifndef CAMERA_AND_CONTROLS_CLASS_H
#define CAMERA_AND_CONTROLS_CLASS_H
#include<iostream>
#include "Libraries//glsl.h"
#include "Shaders/Shader.h"
#include "CameraAndControls.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>


 class CameraAndControls
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	float FOVDeg = glm::radians(45.0f);
	float NearPlane = 0.1f;
	float FarPlane = 100.0f;
	bool firstClick;
	
	int height;
	int width;

	float speed = 0.1f;
	float yaw = -90.0f;
	float pitch = -90.0f;
	float sensitivity = 0.100f;

	CameraAndControls(int width, int height, glm::vec3 position);
	void SetCamera(Shader& shader, const char* uniform);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	void KeyBoardHandler(unsigned char key, int mouseX, int mouseY);
	void MouseHandler(int button, int state, int x, int y);
	void MotionHandler( int x, int y, float deltaTime);
};

#endif // !CAMERA_AND_CONTROLS_CLASS_H


