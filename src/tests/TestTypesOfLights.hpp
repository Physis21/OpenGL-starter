#pragma once

#include <memory>
#include <array>

#include "IndexBuffer.hpp"
#include "Test.hpp"
#include "GLErrorManager.hpp"
#include "glad.h"
#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"

#include "VertexBufferLayout.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"

namespace test {
    class TestTypesOfLights: public Test {
        public:
            TestTypesOfLights(GLFWwindow* window);
            ~TestTypesOfLights();

            void OnUpdate(float deltatime, GLFWwindow* window) override;
            void OnRender() override;
            void OnImGuiRender() override;

        private:
            // pyramid
            std::unique_ptr<VertexArray> m_VAO;
            std::unique_ptr<VertexBuffer> m_VertexBuffer;
            std::unique_ptr<IndexBuffer> m_IndexBuffer;
            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<Texture> m_Texture0;
            std::unique_ptr<Texture> m_Texture1;
            std::unique_ptr<GLuint> m_ShaderChoice = std::make_unique<GLuint>(0);

            // light cube
            std::unique_ptr<VertexArray> m_VAOCube;
            std::unique_ptr<VertexBuffer> m_VertexBufferCube;
            std::unique_ptr<IndexBuffer> m_IndexBufferCube;
            std::unique_ptr<Shader> m_ShaderCube;

            std::unique_ptr<Camera> m_Camera;

            ImGuiIO& m_ImGuiIO;
    };

}
