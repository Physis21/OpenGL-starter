#pragma once

#include <optional>
#include <string>
#include "GLErrorManager.hpp"
#include "glad.h"
#include "stb_image.h"

class Texture {

    private:
        std::string m_Name; // file path by default
        unsigned int m_RendererID;
        std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP; // BPP = bits per pixels
        bool m_FilterNearest;

    public:
        Texture(const std::string& path, bool filterNearest = false, GLint format = GL_RGBA);
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        void SetName(std::string name) { m_Name = name; };
        inline std::string GetName() const { return m_Name; }
        inline int GetWidth() const { return m_Width;}
        inline int GetHeight() const { return m_Height;}
}; 
