#pragma once

#include <optional>
#include <string>
#include "GLErrorManager.hpp"
#include <glad.h>
#include "stb_image.h"

class Texture {

    private:
        std::string m_Name; // file path by default
        unsigned int m_RendererID;
        std::string m_FilePath;
        // e.g. "texture_diffuse" / "texture_specular" (used by models). Not assigned in constructor!
        std::string m_Type;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP; // BPP = bits per pixels
        bool m_FilterNearest;

    public:
        Texture(const std::string& path, bool filterNearest = false, GLint format = GL_RGBA, bool flipVertically = true);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&& other) noexcept:
            m_Name(std::move(other.m_Name)),
            m_RendererID(other.m_RendererID),
            m_FilePath(std::move(other.m_FilePath)),
            m_Type(std::move(other.m_Type)),
            m_LocalBuffer(std::move(other.m_LocalBuffer)),
            m_Width(other.m_Width),
            m_Height(other.m_Height),
            m_BPP(other.m_BPP),
            m_FilterNearest(other.m_FilterNearest)
        {
                other.m_RendererID = 0; // null-out so its destructor is a no-op
                other.m_LocalBuffer = nullptr;
        }
        Texture& operator=(Texture&& other) noexcept {
            if (this != &other) {
                GLCall(glDeleteTextures(1, &m_RendererID)); // free what we currently own
                // Move ALL members so we don't keep stale metadata alongside the new handle.
                m_Name          = std::move(other.m_Name);
                m_RendererID    = other.m_RendererID;
                m_FilePath      = std::move(other.m_FilePath);
                m_Type          = std::move(other.m_Type);
                m_LocalBuffer   = other.m_LocalBuffer;
                m_Width         = other.m_Width;
                m_Height        = other.m_Height;
                m_BPP           = other.m_BPP;
                m_FilterNearest = other.m_FilterNearest;
                // null-out the source's owning handles so its destructor is a no-op
                other.m_RendererID  = 0;
                other.m_LocalBuffer = nullptr;
            }
            return *this;
        }

        /* Selects the unit (glActiveTexture) then binds into it, in the correct order. */
        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        void SetName(std::string name) { m_Name = name; };
        inline std::string GetName() const { return m_Name; }
        void SetType(const std::string& type) { m_Type = type; };
        inline const std::string& GetType() const { return m_Type; }
        inline const std::string& GetFilePath() const { return m_FilePath; }
        inline int GetWidth() const { return m_Width;}
        inline int GetHeight() const { return m_Height;}
}; 
