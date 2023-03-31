#include "CameraAndControls.h"

CameraAndControls::CameraAndControls(int width, int height, glm::vec3 position)
{
	this->width = width;
	this->height = height;
	this->Position = position;
}

void CameraAndControls::SetCamera(Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = GetViewMatrix();
	projection = GetProjectionMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(view * projection));
}

glm::mat4 CameraAndControls::GetViewMatrix() {
	return glm::lookAt(this->Position, this->Position + this->Orientation, this->Up);
}

glm::mat4 CameraAndControls::GetProjectionMatrix()
{
	return glm::perspective(this->FOVDeg, (float)this->width / this->height, this->NearPlane, this->FarPlane);
}

void CameraAndControls::KeyBoardHandler(unsigned char key, int mouseX, int mouseY)
{
	int mod = glutGetModifiers();
	if (mod == 1) {
		speed = 0.5f;
	}
	else {
		speed = 0.01f;
	}
	if (key == 27) {
		glutExit();
	}
	// W going forward
	if (key == 'w') {
		this->Position += this->speed * this->Orientation;
	}
	// S going back;
	if (key == 's') {
		this->Position += this->speed * -this->Orientation;
	}
	// A going left
	if (key == 'a') {
		this->Position += this->speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	// D going right
	if (key == 'd') {
		this->Position += this->speed * glm::normalize(glm::cross(Orientation, Up));
	}
	// q Going up
	if (key == 'q') {
		this->Position += this->speed * this->Up;
	}
	// e Going down
	if (key == 'e') {
		this->Position += this->speed * -this->Up;
	}
}
void CameraAndControls::MouseHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	else {
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		firstClick = true;
	}
}

void CameraAndControls::MotionHandler(int x, int y, float deltaTime) {
	
	if (firstClick)
	{
		glutWarpPointer(this->width / 2, this->height / 2);
		firstClick = false;
	}


	float xoffset = this->sensitivity * deltaTime * (float)(x - (this->height / 2) / this->height);
	float yoffset = this->sensitivity * deltaTime * (float)(y - (this->width / 2) / this->width);

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Orientation = glm::normalize(direction);



	//if (firstClick) {
	//	
	//	firstClick = true;
	//}
	//float rotX = this->sensitivity * deltaTime * (float)(x - (this->height / 2) / this->height);
	//float rotY = this->sensitivity * deltaTime * (float)(y - (this->width / 2) / this->width);

	////prevent camera from doing a barrel roll
	//glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, glm::vec3(0.0, 1.0, 0.0))));
	////glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::vec3(1.0, 0.0, 0.0));

	//if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	//{
	//	Orientation = newOrientation;
	//}
	//
	//// Rotates the Orientation left and right
	//Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

	//glutWarpPointer(this->width / 2, this->height / 2);

}
