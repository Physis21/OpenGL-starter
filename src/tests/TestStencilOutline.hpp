#pragma once

#include <memory>
#include <array>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include <glad.h>
#include <imgui.h>

#include "Test.hpp"
#include "GLErrorManager.hpp"

#include "Renderer.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Model.hpp"


namespace test {
    class TestStencilOutline: public Test {
        public:
            TestStencilOutline(GLFWwindow* window);
            ~TestStencilOutline();

            void OnUpdate(float deltatime, GLFWwindow* window) override;
            void OnRender() override;
            void OnImGuiRender() override;

        private:
            enum ShaderChoice {
                RegularShaderChoice = 0,
                StencilOutlineShaderChoice = 1,
                NormalOutlineShaderChoice = 2,
                SobelShaderChoice = 3,
                Last // Kept only for iterating over enum
            };
            ShaderChoice m_ShaderChoice = RegularShaderChoice;
            // pyramid
            std::unique_ptr<Model> m_Model;
            std::unique_ptr<Shader> m_Shader; // Regular shader
            std::unique_ptr<Shader> m_NormalOutlineShader;
            std::unique_ptr<Shader> m_SobelShader;
            std::unique_ptr<Shader> m_OutlineShader;

            

            std::unique_ptr<Camera> m_Camera;

            ImGuiIO& m_ImGuiIO;
    };

}
