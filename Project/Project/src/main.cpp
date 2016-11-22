#include <iostream>

/**OpenGL Includes**/
#define GLFW_DLL

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "Triangle.h"
#include "NuanceurProg.h"
#include "Scene.h"
#include "Camera.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    // Catch ESC key
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Load shaders
    CNuanceurProg triangleShader{ "shaders/triangle.vert", "shaders/triangle.frag", true };

    // Create our scene
    Scene scene;

    // Create our Camera
    Camera camera;

    // Set all its parameters
    camera.setPosition(glm::vec3(4.0f, 3.0f, 3.0f));

    // Add the camera to the scene
    scene.setCamera(camera);

    // Create our triangle
    Triangle triangle(triangleShader);

    // Add it to the scene
    scene.addRenderable(&triangle);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}