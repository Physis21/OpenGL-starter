#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"


struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    enum class ShaderType {
        NONE=-1, VERTEX=0, FRAGMENT=1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType shaderType = ShaderType::NONE;
    while (std::getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos ) {
                // set mode to vertex
                shaderType = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                shaderType = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)shaderType] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int shaderType, const std::string& source) {
    unsigned int id = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int compile_result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compile_result);
    if (compile_result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // alloca allows for creation of arbitrary-size string on stack!
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr
            << "Failed to compile "
            << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!\n";
        std::cerr << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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

    /* Square with 2 triangles */
    float positions[] = {
        -0.5, -0.5, // 0
         0.5, -0.5, // 1
         0.5,  0.5, // 2
        -0.5,  0.5, // 3
    };

    /* indices HAVE to be unsigned */
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    /* Create a scope to call destructors of objects before the glfw termination deletes the OpenGL context */
    {

        unsigned int vao;

        VertexArray va;
        VertexBuffer vb(positions, 8 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        /* Heap-allocating buffers is pretty common */

        
        std::unique_ptr<IndexBuffer> ib = std::make_unique<IndexBuffer>(indices, 6);


        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        std::cout <<"Vertex source:\n" << source.VertexSource << std::endl;
        std::cout <<"Fragment source:\n" << source.FragmentSource << std::endl;

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        MyGLCall(glUseProgram(shader));


        MyGLCall(int u_Color_location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(u_Color_location != -1); // If uniform not found

        /* Unbind buffers and program */
        // TODO: stopped here at Cherno video
        MyGLCall(glBindVertexArray(0));
        MyGLCall(glUseProgram(0));
        MyGLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        MyGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float rChannel = 0.0f; // Red color channel
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /*
             * GL_TRIANGLES tells GL to group the vertices by 3
             * 0 means you start at the first vertex
             * 3 means that we have 3 indices to render 
            */
            // glDrawArrays(GL_TRIANGLES, 0, 6);

            /* Rebind buffer */
            MyGLCall(glUseProgram(shader));
            MyGLCall(glUniform4f(u_Color_location, rChannel, 0.3f, 0.8f, 1.0f));

            va.Bind();
            ib->Bind();
            // MyGLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
            // MyGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

            MyGLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (rChannel > 1.0f) {
                increment = -0.05f;
            } else if (rChannel < 0.0f){
                increment = 0.05f;
            }

            rChannel += increment;

            glfwSwapBuffers(window);
        }

        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}
