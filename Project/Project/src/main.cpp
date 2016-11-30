#include <iostream>

/**OpenGL Includes**/
#define GLFW_DLL

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>


#include "Triangle.h"
#include "Cube.h"
#include "NuanceurProg.h"
#include "Scene.h"
#include "Camera.h"
#include "Line.h"
#include "../include/Control.h"
#include "Watergrid.h"

#include <ctime>
#include <Windows.h>

const unsigned int width = 1280;
const unsigned int height = 720;
const unsigned int maximum_fps = 20;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glewExperimental = true; // Needed in core profile

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Check if OpenGL 4.0 OK
    if (glewIsSupported("GL_VERSION_4_0"))
        std::cout << "Ready for OpenGL 4.0" << std::endl << std::endl;
    else {
        std::cerr << "OpenGL 4.0 not supported" << std::endl << std::endl;
        exit(1);
    }

    // Specifier le context openGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glViewport(0, 0, width, height);

    // Catch ESC key
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Load shaders
    CNuanceurProg triangleShader{ "shaders/triangle.vert", "shaders/triangle.frag", true };
    CNuanceurProg cubeShader{ "shaders/cube.vert", "shaders/cube.frag", true };
    CNuanceurProg lineShader{ "shaders/line.vert", "shaders/line.frag", true };
    CNuanceurProg waterGridShader{ "shaders/waterGrid.vert", "shaders/waterGrid.frag", true };

    // Create our scene
    Scene scene;

    // Create our Camera
    Camera camera;

    // Set all its parameters
    camera.setPosition(glm::vec3(4.0f, 3.0f, 3.0f));

    // Add the camera to the scene
    scene.setCamera(camera);

    // Create our triangle
    //Triangle triangle(triangleShader);

    // Add it to the scene
    //scene.addRenderable(&triangle);

    // Create cube  
    float cubeEdgeLength = 3.0f;
    Cube cube1(cubeShader, glm::vec3(0.0f, 0.0f, 0.0f), cubeEdgeLength);
    scene.addRenderable(&cube1);
    /*Line line1(lineShader, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1);
    scene.addRenderable(&line1);
    Line line2(lineShader, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1);
    scene.addRenderable(&line2);*/

    // Create watergrid
    WaterGrid waterGrid(waterGridShader, glm::vec3(0.0f, 0.0f, 0.9f), 50, 50, cubeEdgeLength, cubeEdgeLength);
    waterGrid.setCube(&cube1);
    cube1.setWaterGrid(&waterGrid);
    scene.addRenderable(&waterGrid);

    // Create the control
    Control& control = Control::getInstance();
    control.setWindowAndScene(window, &scene);
    control.setGrid(&waterGrid);

    //glViewport(0, 0, width, height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        clock_t begin = clock();

        /* Render here */
        scene.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        // Camera handle (mouse + key arrows)
        control.processEvents();

        /* Poll for and process events */
        glfwPollEvents();

        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        if (elapsed_secs < 1.0 / maximum_fps) {
            Sleep(long((1.0 / maximum_fps - elapsed_secs) * 1000));
        }
    }

    glfwTerminate();
    return 0;
}