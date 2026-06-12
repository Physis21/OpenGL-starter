#pragma once
#include <cstddef>
#include <iostream>
#include <signal.h>
#include <vector>
#include "glad.h"

#define ASSERT(x) if (!(x)) raise(SIGTRAP)
// #x convert x to a string
#define GLCall(x) MyGLClearError();\
    x;\
    ASSERT(MyGLLogCall(#x, __FILE__, __LINE__))

void MyGLClearError();

bool MyGLLogCall(const char* functionName, const char* file, int line);

template <typename T>
void PrintVector(std::vector<T> vector) {
    for (int i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << ", ";
    }
    std::cout << std::endl;
}
