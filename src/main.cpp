#include "main.hpp"

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
        -200, -240, 0.0f, 0.0f, // 0
         200, -240, 1.0f, 0.0f, // 1
         200,  240, 1.0f, 1.0f, // 2
        -200,  240, 0.0f, 1.0f, // 3
    };

    /* indices HAVE to be unsigned */
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    /* Blending 
     * TODO: move into Renderer init?*/
    GLCall(glEnable(GL_BLEND));
    /* Default: glBlendFunc(src=GL_ONE, dest=GL_ZERO) 
     * The source gets overriden by zeros in this case,
     * and the default glBlendEquation(GL_FUNC_ADD) simply adds src+dest 
     * NOTE: the alpha value also goes through the equation */
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


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

        /* Here we define +-1.2f x bounds, +-1.0f y bounds 
         * This 0 is indeed the middle of the screen */
        glm::mat4 proj = glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", proj);

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

            // TODO: shader bind already happens in draw call, these 2 lines can probably go away
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
