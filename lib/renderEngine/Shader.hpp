#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>

#include <glad.h>
#include "GLErrorManager.hpp"
#include "glm/glm.hpp"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {

    private:
        std::string m_FilePath;
        unsigned int m_RendererID;
        // caching for uniforms
        mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;


    public:
        Shader(const std::string& filepath);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&& other) noexcept
            : m_RendererID(other.m_RendererID), m_FilePath(std::move(other.m_FilePath)) {
                other.m_RendererID = 0; // null-out so its destructor is a no-op
            }
        Shader& operator=(Shader&& other) noexcept {
            if (this != &other) {
                GLCall(glDeleteProgram(m_RendererID));
                m_RendererID = other.m_RendererID;
                m_FilePath = std::move(other.m_FilePath);
                other.m_RendererID = 0;
            }
            return *this;
        }

        unsigned int GetRendererID() const;

        void Bind() const;
        void Unbind() const;
    
        // Set uniforms;
        void SetUniform1ui(const std::string& name, int value);
        void SetUniform1i(const std::string& name, int value);
        void SetUniform1f(const std::string& name, float value);
        void SetUniform3f(const std::string& name, float v0, float v1, float v2);
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
        void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);


    private:
        ShaderProgramSource ParseShader(const std::string& filepath);
        /* Create the shader, assign the source (code text) to it and compile it.
         * If the shader could not be compiled, log an error and return 0.
         * @param[in] shaderType the type of shader (e.g GL_FRAGMENT_SHADER)
         * @param[in] source the source code for the shader
         * @returns the id of the compiled shader.
         */
        unsigned int CompileShader(unsigned int shaderType, const std::string& source);
        /* Compile the vertex and fragment shader, attach them to the
         * shader program pointed by m_RendererID, link the program object
         * then validate it.
         * The fragment and vertex shaders are then detached from the program and deleted.
         * @param[in] vertexShader the code for the vertex shader
         * @param[in] fragmentShader the code for the fragment shader
         * @returns the program number.
         */
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        GLint GetUniformLocation(const std::string& name) const;

};
