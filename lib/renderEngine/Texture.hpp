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

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&& other) noexcept:
            m_Name(std::move(other.m_Name)),
            m_RendererID(other.m_RendererID),
            m_FilePath(std::move(other.m_FilePath)),
            m_LocalBuffer(std::move(other.m_LocalBuffer)),
            m_Width(other.m_Width),
            m_Height(other.m_Height),
            m_BPP(other.m_BPP),
            m_FilterNearest(other.m_FilterNearest)
        {
                other.m_RendererID = 0; // null-out so its destructor is a no-op
        }
        Texture& operator=(Texture&& other) noexcept {
            if (this != &other) {
                GLCall(glDeleteTextures(1, &m_RendererID)); // free what we currently own
                m_RendererID = other.m_RendererID;
                other.m_RendererID = 0;
            }
            return *this;
        }

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        void SetName(std::string name) { m_Name = name; };
        inline std::string GetName() const { return m_Name; }
        inline int GetWidth() const { return m_Width;}
        inline int GetHeight() const { return m_Height;}
}; 
