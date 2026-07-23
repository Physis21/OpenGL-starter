#pragma once

#include <optional>
#include <string>
#include <utility> // for std::swap
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

        /* "Empty but valid" state, used only as the starting point for a move.
         * Owns no GL handle, so its destructor is a no-op. */
        Texture() noexcept
            : m_Name(), m_RendererID(0), m_FilePath(), m_Type(),
              m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0),
              m_FilterNearest(false) {}

    public:
        Texture(const std::string& path, bool filterNearest = false, GLint format = GL_RGBA, bool flipVertically = true);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        /* Move via swap: start empty, then swap our state with `other`,
         * leaving `other` empty and safely destructible. */
        Texture(Texture&& other) noexcept : Texture() {
            swap(*this, other);
        }
        /* Swap with the incoming rvalue: our old resource ends up in `other`,
         * which the caller destroys - that is what frees it. Self-move is a no-op. */
        Texture& operator=(Texture&& other) noexcept {
            swap(*this, other);
            return *this;
        }

        /* Single place that knows every member. Found by ADL, so `std::swap(a, b)`
         * and generic code pick this up. */
        friend void swap(Texture& a, Texture& b) noexcept {
            using std::swap;
            swap(a.m_Name,          b.m_Name);
            swap(a.m_RendererID,    b.m_RendererID);
            swap(a.m_FilePath,      b.m_FilePath);
            swap(a.m_Type,          b.m_Type);
            swap(a.m_LocalBuffer,   b.m_LocalBuffer);
            swap(a.m_Width,         b.m_Width);
            swap(a.m_Height,        b.m_Height);
            swap(a.m_BPP,           b.m_BPP);
            swap(a.m_FilterNearest, b.m_FilterNearest);
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
