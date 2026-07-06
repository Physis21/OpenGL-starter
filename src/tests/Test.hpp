#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

#include "imgui.h"
#include "glad.h"

#include "GLErrorManager.hpp"

namespace test {
    
    class Test {
        public:
            Test() {}
            virtual ~Test() {}
            
            // Not pure virtual so that we don't **have** to implement them
            
            virtual void OnUpdate(float deltaTime, float display_w, float display_h) {}
            virtual void OnRender() {}
            virtual void OnImGuiRender() {}

    };

    class TestMenu: public Test {
        public:
            TestMenu(Test*& currentTestPointer);
            virtual ~TestMenu();

            virtual void OnUpdate(float deltaTime, float display_w, float display_h) override;
            virtual void OnRender() override;
            virtual void OnImGuiRender() override;

            template<typename T>
            void RegisterTest(const std::string& name) {
                std::cout << "Registering test " << name << std::endl;
                m_Tests.push_back(std::make_pair(name, []() {return new T();}));
            }

        private:
            Test*& m_CurrentTest; // Reference to the test pointer, in order to write to that pointer
            /* functions which return Test pointers */
            std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
    };
}
