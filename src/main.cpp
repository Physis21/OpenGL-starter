#include <iostream>
#include "GLErrorManager.hpp"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"



int main(int argc, char**argv){
    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "openGL-starter window!", NULL, NULL);
    if (!window) {
        std::cerr << "Couldn't open Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Create the OpenGL context

    glfwSwapInterval(10); // Limit framerate

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Couldn't open OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    /* Square with 2 triangles 
     * 2 positions, then 2 texture coordinates */
    float positions[] = {
        -0.5, -0.5, 0.0f, 0.0f, // 0
         0.5, -0.5, 1.0f, 0.0f, // 1
         0.5,  0.5, 1.0f, 1.0f, // 2
        -0.5,  0.5, 0.0f, 1.0f, // 3
    };

    /* indices HAVE to be unsigned */
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    /* Blending */
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));


    /* Create a scope to call destructors of objects before the glfw termination deletes the OpenGL context */
    {

        unsigned int vao;

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        /* Heap-allocating buffers is pretty common */

        
        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/TsukihimeLogo.png");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0); // 0 because we bind the texture to slot 0

        /* Unbind buffers and program */
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float rChannel = 0.0f; // Red color channel
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4f("u_Color", rChannel, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (rChannel > 1.0f)
                increment = -0.05f;
            else if (rChannel < 0.0f)
                increment = 0.05f;

            rChannel += increment;

            glfwSwapBuffers(window);
        }
    }
    glfwTerminate();
    return 0;
}
