#pragma once
#include <iostream>
#include <signal.h>
#include "glad.h"

#define ASSERT(x) if (!(x)) raise(SIGTRAP)
// #x convert x to a string
#define GLCall(x) MyGLClearError();\
    x;\
    ASSERT(MyGLLogCall(#x, __FILE__, __LINE__))

void MyGLClearError();

bool MyGLLogCall(const char* functionName, const char* file, int line);
