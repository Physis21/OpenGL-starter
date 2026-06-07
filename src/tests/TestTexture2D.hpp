#pragma once

#include <memory>

#include "IndexBuffer.hpp"
#include "Test.hpp"
#include "GLErrorManager.hpp"
#include "glad.h"
#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBufferLayout.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"


namespace test {

    class TestTexture2D: public Test {
        public:
            TestTexture2D();
            ~TestTexture2D();

            void OnUpdate(float deltatime) override;
            void OnRender() override;
            void OnImGuiRender() override;

        private:
            std::unique_ptr<VertexArray> m_VAO;
            std::unique_ptr<VertexBuffer> m_VertexBuffer;
            std::unique_ptr<IndexBuffer> m_IndexBuffer;
            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<Texture> m_Texture;

            glm::mat4 m_Proj, m_View;
            glm::vec3 m_TranslationA, m_TranslationB;

            bool m_ShowAnotherWindow;
            float m_ClearColor[4];
            ImGuiIO& m_ImGuiIO;
    };

}
