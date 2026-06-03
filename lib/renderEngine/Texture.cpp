#include "Texture.hpp"

Texture::Texture(const std::string& path) 
: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0){
    /* In OpenGL, the bottom left is 0, 0
     * while in stb, the top left is 0, 0
     */
    stbi_set_flip_vertically_on_load(1);
    /* stbi_load writes the width and height to m_Width and m_Height, usign their references
     * 4 desired channels for RGBA */
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    /* Generate and bind texture, and store its ID in m_RendererID */
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    /* NOTE: Without these 4 parameters, you will get a black texture */

    /* MIN_FILTER is how the texture is minimized, and GL_LINEAR means we lineary resample everything */
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    /* MAG_FILTER is how the texture is magnified */
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Pass the image data to OpenGl
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    // Unbind the texture
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer) // If Buffer does contain data, free it
        stbi_image_free(m_LocalBuffer);
} 
Texture::~Texture() {
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // add to get a texture slot. There are 32 texture slots for PC, probably 8 for mobile.
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}
void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

