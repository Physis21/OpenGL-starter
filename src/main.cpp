#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <array>
#include <fstream>
#include <string>
#include <sstream>

#define LOG(x) std::cout << x << std::endl;

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
        std::cout 
            << "Failed to compile "
            << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!\n";
        std::cout << message << std::endl;
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

    window = glfwCreateWindow(640, 480, "openGL-starter window!", NULL, NULL);
    if (!window) {
        std::cout << "Couldn't open Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Create the OpenGL context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Couldn't open OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::array<float, 6> positions = {
        -0.5, -0.5,
        0.0, 0.5,
        0.5, -0.5,
    };

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    /* ID of generated buffer. After glGenBuffers, it will have the ID of the generated Buffer */
    unsigned int buffer; 
    glGenBuffers(1, &buffer);
    LOG("Current buffer:"); LOG(buffer);
    /* Select a buffer (binding)
    GL_ARRAY_BUFFER tells the GPU this is just a buffer of memory */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    /* Buffer size should be in bytes, GL_STATIC_DRAW is a hint to the GL implementation */
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

    /* 0 is the index of the generic vertex attribute to be enabled or disabled */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*) 0);


    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main() {\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main() {\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    std::cout <<"Vertex source:\n" << source.VertexSource << std::endl;
    std::cout <<"Fragment source:\n" << source.FragmentSource << std::endl;

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*
         * GL_TRIANGLES tells GL to group the vertices by 3
         * 0 means you start at the first vertex
         * means 3 that we have 3 indices to render 
        */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
