#include "Texture.hpp"

Texture::Texture(const std::string& path, bool filterNearest, GLint format, bool flipVertically)
: m_Name(path), m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_FilterNearest(filterNearest){
    /* In OpenGL, the bottom left is 0, 0
     * while in stb, the top left is 0, 0.
     * stb's flip state is GLOBAL, so we set it explicitly on every load
     * instead of relying on whatever the previous Texture left behind. */
    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);
    /* stbi_load writes the width and height to m_Width and m_Height, usign their references
     * if wanted to force 4 channels for RGBA, set 4 as the last parameter.
     * Else, set 0 so that stbi adapts to the image. */
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);

    /* Generate and bind texture, and store its ID in m_RendererID */
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    /* NOTE: Without these 4 parameters, you will get a black texture */

    /* MIN_FILTER is how the texture is minimized, and GL_LINEAR means we lineary resample everything */
    GLuint filter;
    if (filterNearest) {filter = GL_NEAREST;} else {filter = GL_LINEAR;}
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
    /* MAG_FILTER is how the texture is magnified */
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Tell OpenGL how to repeat the texture for values > +-1, for axises S and T
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    // Pass the image data to OpenGl
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));

    // Generate mipmaps which are smaller resolution versions of the texture when it is far away
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    // Unbind the texture
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer) // If Buffer does contain data, free it
        stbi_image_free(m_LocalBuffer);
} 
Texture::~Texture() {
    if (m_RendererID != 0)
        GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // add to get a texture slot. There are 32 texture slots for PC, probably 8 for mobile.
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}
void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

