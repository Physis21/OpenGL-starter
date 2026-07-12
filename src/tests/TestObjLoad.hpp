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
#include "ObjLoader.hpp"

#include "tiny_obj_loader.h"


namespace test {
    class TestObjLoad: public Test {
        struct XYRotation {
            float x;
            float y;
        };
        public:
            TestObjLoad(GLFWwindow* window);
            ~TestObjLoad();

            void OnUpdate(float deltatime, GLFWwindow* window) override;
            void OnRender() override;
            void OnImGuiRender() override;

        private:
            // TODO: change VertexBuffer and IndexBuffer to unique_ptr to ObjLoader
            std::unique_ptr<VertexArray> m_VAO;
            std::unique_ptr<VertexBuffer> m_VertexBuffer;
            std::unique_ptr<IndexBuffer> m_IndexBuffer;
            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<Texture> m_Texture;

            // left, right, bottom, top, near, far. near/far are distances along the -Z axis in view space!
            glm::mat4 m_Proj;
            glm::mat4 m_View;
            glm::vec3 m_TranslationA, m_TranslationB;
            XYRotation m_XYRotationA, m_XYRotationB;

            ImGuiIO& m_ImGuiIO;
            glm::vec4 m_LightDirection;
    };

}
