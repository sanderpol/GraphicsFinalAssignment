#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Libraries/glsl.h"
#include "Libraries/objloader.h"

#include "WorldRepository.h"
#include "CameraAndControls.h"
#include "Skybox.h"




//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int16_t WIDTH = 1280, HEIGHT = 720;
const float DELTA_TIME = 60.0f;

WorldRepository* worldRep;
Skybox* skybox;
CameraAndControls* camera;
float deltaTime;

//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    // Define background
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    //calculate deltaTime
    static float prevTime = 0.0f;
    float currTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currTime - prevTime;
    prevTime = currTime;

    deltaTime /= 60.0f;

    worldRep->Render(*camera);
    //skybox->Render(*camera);



    // Swap buffers
    glutSwapBuffers();

    int errorCode = glGetError();
    std::cout << errorCode << endl;
}

/// <summary>
/// hadnles the keyEvent through to the camera class, the glutKeyboardFunc only is able to handle staic functions.
/// </summary>
void KeyBoardHandler(unsigned char key, int mouseX, int mouseY) {
    camera->KeyBoardHandler(key, mouseX, mouseY);
}

void MouseHandler(int button, int state, int x, int y) {
    camera->MouseHandler(button, state, x, y);
}

void MotionHandler(int x, int y) {
    camera->MotionHandler(x, y, deltaTime);
}
//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void Render(int n)
{
    Render();
    glutTimerFunc(DELTA_TIME, Render, 0);
}

//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Graphics Project");
    glutDisplayFunc(Render);


    glutKeyboardFunc(KeyBoardHandler);
    glutMouseFunc(MouseHandler);
    glutMotionFunc(MotionHandler);
    glutTimerFunc(DELTA_TIME, Render, 0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT); 

    glewInit();
}

//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------
void InitBuffers() {
    //WorldRepository::InitBuffers(projection);
    //skybox->InitSkyboxShadersAndBuffers();
}


int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);
    //WorldRepository::InitializeWorldRepository();

    camera = new CameraAndControls(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    worldRep = new WorldRepository();
    //worldRep->InitializeWorldRepository();
    //skybox = new Skybox();
    InitBuffers();
    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    //std::cin.get();
    // Main loop
    glutMainLoop();

    return 0;
}
