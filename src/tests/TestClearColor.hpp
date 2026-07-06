#pragma once

#include "Test.hpp"
#include "GLErrorManager.hpp"
#include "glad.h"
#include "imgui.h"

namespace test {

    class TestClearColor: public Test {
        public:
            TestClearColor();
            ~TestClearColor();

            void OnUpdate(float deltatime, float display_w, float display_h) override;
            void OnRender() override;
            void OnImGuiRender() override;

        private:
            float m_ClearColor[4];
    };

}
