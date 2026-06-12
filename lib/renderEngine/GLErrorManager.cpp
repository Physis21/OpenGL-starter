#include "GLErrorManager.hpp"
#include <ostream>

void MyGLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool MyGLLogCall(const char* functionName, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cerr << "[OpenGL Error] (" << error << ") " << functionName
            << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
} 
