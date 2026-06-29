#pragma once

#include <string>
#include "GLErrorManager.hpp"
#include "glad.h"
#include "stb_image.h"
// TODO: why do I need to add stb_image as a dependency of main.cpp?

class Texture {

    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP; // BPP = bits per pixels
        bool m_FilterNearest;

    public:
        Texture(const std::string& path, bool filterNearest = false);
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        inline int GetWidth() const { return m_Width;}
        inline int GetHeight() const { return m_Height;}
}; 
