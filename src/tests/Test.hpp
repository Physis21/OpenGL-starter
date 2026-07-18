#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "GLErrorManager.hpp"

namespace test {
    
    class Test {
        public:
            Test() {}
            virtual ~Test() {}
            
            // Not pure virtual so that we don't **have** to implement them
            
            /*
             * @param[in] deltaTime the elapsed time (seconds) since the last frame.
             * @param[inout] window the GLFW window pointer
             */
            virtual void OnUpdate(float deltaTime, GLFWwindow* window) {}
            virtual void OnRender() {}
            virtual void OnImGuiRender() {}

    };

    class TestMenu: public Test {
        public:
            TestMenu(Test*& currentTestPointer);
            virtual ~TestMenu();

            virtual void OnUpdate(float deltaTime, GLFWwindow* window) override;
            virtual void OnRender() override;
            virtual void OnImGuiRender() override;

            template<typename T>
            void RegisterTest(const std::string& name, GLFWwindow* window) {
                std::cout << "Registering test " << name << std::endl;
                m_Tests.push_back(std::make_pair(name, [window]() {return new T(window);}));
            }

        private:
            Test*& m_CurrentTest; // Reference to the test pointer, in order to write to that pointer
            /* functions which return Test pointers */
            std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
    };
}
