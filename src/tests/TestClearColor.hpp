#pragma once

#include "Test.hpp"
#include "GLErrorManager.hpp"
#include <glad.h>
#include <imgui.h>

namespace test {

    class TestClearColor: public Test {
        public:
            TestClearColor(GLFWwindow* window);
            ~TestClearColor();

            void OnUpdate(float deltatime, GLFWwindow* window) override;
            void OnRender() override;
            void OnImGuiRender() override;

        private:
            float m_ClearColor[4];
    };

}
