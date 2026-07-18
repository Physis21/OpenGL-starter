#pragma once

#include <memory>
#include <array>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glad.h"
#include "imgui.h"

#include "Test.hpp"
#include "GLErrorManager.hpp"

#include "Renderer.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Model.hpp"

namespace test {
    class TestModelLoad: public Test {
        public:
            TestModelLoad(GLFWwindow* window);
            ~TestModelLoad();

            void OnUpdate(float deltatime, GLFWwindow* window) override;
            void OnRender() override;
            void OnImGuiRender() override;

        private:
            // pyramid
            std::unique_ptr<Model> m_Model;
            std::unique_ptr<Shader> m_Shader;

            std::unique_ptr<Camera> m_Camera;

            ImGuiIO& m_ImGuiIO;
    };

}
