#include "Test.hpp"

namespace test {
    TestMenu::TestMenu(Test*& currentTestPointer)
    : m_CurrentTest(currentTestPointer) {

    }
    TestMenu::~TestMenu(){

    }

    void TestMenu::OnUpdate(float deltaTime){

    }
    void TestMenu::OnRender(){
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    void TestMenu::OnImGuiRender(){
        for (auto& test: m_Tests) {
            if(ImGui::Button(test.first.c_str()))
                m_CurrentTest = test.second();
        }
    }

}
